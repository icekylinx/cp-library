#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct SparseTable {
  using Info = typename T::Info;

  uint32_t n = 0;
  int h = 0;
  std::vector<std::vector<Info>> st;

  SparseTable() = default;

  template <typename It>
  SparseTable(It first, It last) { build(first, last); }

  template <typename F>
  SparseTable(uint32_t _n, F&& func) { build(_n, func); }

  template <typename It>
  void build(It first, It last) {
    int _n = std::distance(first, last);
    CHECK(_n >= 0);
    std::vector<Info> vec(first, last);
    build(static_cast<uint32_t>(_n), [&](uint32_t i) {
      return vec[i];
    });
  }

  template <typename F>
  void build(uint32_t _n, F&& func) {
    n = _n;
    if (n == 0) {
      h = 0;
      st.clear();
      return;
    }

    h = std::bit_width(n);
    st.resize(h);

    st[0].resize(n);
    for (uint32_t i = 0; i < n; ++i) {
      st[0][i] = func(i);
    }

    for (int i = 1; i < h; ++i) {
      uint32_t len = 1 << i;
      uint32_t half = len >> 1;
      st[i].resize(n - len + 1);
      for (uint32_t j = 0; j < n - len + 1; ++j) {
        st[i][j] = T::op(st[i - 1][j], st[i - 1][j + half]);
      } 
    }
  }

  Info prod(uint32_t l, uint32_t r) const {
    CHECK(l <= r && r <= n);
    if (l == r) [[unlikely]] return T::id();
    int k = std::bit_width(r - l) - 1;
    return T::op(st[k][l], st[k][r - (1 << k)]);
  }
};