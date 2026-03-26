#pragma once

#include "lib/utils/debug.hpp"

template <typename T, uint32_t B = 16>
struct BlockedSparseTable {
  using Info = typename T::Info;

  uint32_t n = 0;
  uint32_t m = 0;
  uint32_t size = 0;
  int h = 0;
  std::vector<std::vector<Info>> st;
  std::vector<Info> val, pre, suf;

  BlockedSparseTable() = default;

  template <typename It>
  BlockedSparseTable(It first, It last) { build(first, last); }

  template <typename F>
  BlockedSparseTable(uint32_t _n, F&& func) { build(_n, func); }

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

    size = (n + B - 1) / B;
    m = size * B;
    h = std::bit_width(size);
    st.resize(h);
    st[0].resize(size);
    val.assign(m, T::id());
    pre.assign(m, T::id());
    suf.assign(m, T::id());

    for (uint32_t i = 0; i < n; ++i) {
      val[i] = func(i);
      pre[i] = i % B == 0 ? val[i] : T::op(pre[i - 1], val[i]);
    }

    for (uint32_t i = 0; i < m; i += B) {
      uint32_t down = i, up = i + B - 1;
      suf[up] = val[up];
      while (up > down) {
        --up;
        suf[up] = T::op(val[up], suf[up + 1]);
      }
      st[0][i / B] = suf[down];
    }

    for (int i = 1; i < h; ++i) {
      uint32_t len = 1 << i;
      uint32_t half = len >> 1;
      st[i].resize(size - len + 1);
      for (uint32_t j = 0; j < size - len + 1; ++j) {
        st[i][j] = T::op(st[i - 1][j], st[i - 1][j + half]);
      } 
    }
  }

  Info prod(uint32_t l, uint32_t r) const {
    CHECK(l <= r && r <= n);
    if (l == r) [[unlikely]] return T::id();
    --r;
    uint32_t L = l / B, R = r / B;
    if (L == R) {
      Info res = val[l];
      for (uint32_t i = l + 1; i <= r; ++i) {
        res = T::op(res, val[i]);
      }
      return res;
    } else if (L == R - 1) {
      return T::op(suf[l], pre[r]);
    } else {
      ++L;
      int k = std::bit_width(R - L) - 1;
      return T::op(T::op(suf[l], st[k][L]), T::op(st[k][R - (1 << k)], pre[r]));
    }
  }
};