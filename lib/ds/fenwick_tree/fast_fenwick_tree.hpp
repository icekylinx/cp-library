#pragma once

#include "lib/utils/debug.hpp"

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/#fenwick-trees
template <typename T>
struct FastFenwickTree {
  using Info = typename T::Info;

  uint32_t n = 0;
  std::vector<Info> t;

  static constexpr uint32_t hole(uint32_t x) {
    return x + (x >> 10);
  }

  FastFenwickTree() = default;
  explicit FastFenwickTree(uint32_t _n) { build(_n); }

  template <typename F>
  FastFenwickTree(uint32_t _n, F&& func) { build(_n, func); }

  template <typename It>
  FastFenwickTree(It first, It last) { build(first, last); }

  void build(uint32_t _n) {
    n = _n;
    t.assign(hole(n) + 1, T::id());
  }

  template <typename F>
  void build(uint32_t _n, F&& func) {
    build(_n);
    std::vector<Info> pre(n + 1);
    Info sum = T::id();
    for (uint32_t i = 1; i <= n; ++i) {
      pre[i] = sum = T::op(sum, func(i - 1));
      t[hole(i)] = T::op(pre[i], T::inv(pre[i & (i - 1)]));
    }
  }

  template <typename It>
  void build(It first, It last) {
    build(static_cast<uint32_t>(std::distance(first, last)));
    std::vector<Info> pre(n + 1);
    Info sum = T::id();
    for (uint32_t i = 1; i <= n; ++i, ++first) {
      pre[i] = sum = T::op(sum, *first);
      t[hole(i)] = T::op(pre[i], T::inv(pre[i & (i - 1)]));
    }
  }

  void add(uint32_t x, const Info& v) {
    CHECK(x < n);
    for (++x; x <= n; x += x & -x) {
      t[hole(x)] = T::op(t[hole(x)], v);
    }
  }

  Info sum(uint32_t x) const {
    CHECK(x <= n);
    Info res = T::id();
    for (; x > 0; x &= x - 1) {
      res = T::op(res, t[hole(x)]);
    }
    return res;
  }

  Info sum(uint32_t l, uint32_t r) const {
    CHECK(l <= r && r <= n);
    return T::op(sum(r), T::inv(sum(l)));
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
  }
};