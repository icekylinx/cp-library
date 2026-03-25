#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct FenwickTreeRange {
  using Info = typename T::Info;

  uint32_t n = 0;
  std::vector<Info> t1, t2;

  FenwickTreeRange() = default;
  explicit FenwickTreeRange(uint32_t _n) { build(_n); }

  template <typename F>
  FenwickTreeRange(uint32_t _n, F&& func) { build(_n, func); }

  template <typename It>
  FenwickTreeRange(It first, It last) { build(first, last); }

  void build(uint32_t _n) {
    n = _n;
    t1.assign(n + 1, T::id());
    t2.assign(n + 1, T::id());
  }

  template <typename F>
  void build(uint32_t _n, F&& func) {
    build(_n);
    std::vector<Info> pre(n + 1);
    Info sum = T::id();
    for (uint32_t i = 1; i <= n; ++i) {
      pre[i] = sum = T::op(sum, func(i - 1));
      t2[i] = T::op(pre[i], T::inv(pre[i & (i - 1)]));
    }
  }

  template <typename It>
  void build(It first, It last) {
    build(static_cast<uint32_t>(std::distance(first, last)));
    std::vector<Info> pre(n + 1);
    Info sum = T::id();
    for (uint32_t i = 1; i <= n; ++i, ++first) {
      pre[i] = sum = T::op(sum, *first);
      t2[i] = T::op(pre[i], T::inv(pre[i & (i - 1)]));
    }
  }

  void add(uint32_t l, uint32_t r, const Info& v) {
    CHECK(l <= r && r <= n);
    ++l, ++r;
    add_one(t1, n, l, v);
    add_one(t1, n, r, T::inv(v));
    add_one(t2, n, l, T::pow(T::inv(v), l - 1));
    add_one(t2, n, r, T::pow(v, r - 1));
  }

  Info sum(uint32_t x) const {
    CHECK(x <= n);
    Info s1 = sum_one(t1, x);
    Info s2 = sum_one(t2, x);
    return T::op(T::pow(s1, x), s2);
  }

  Info sum(uint32_t l, uint32_t r) const {
    CHECK(l <= r && r <= n);
    return T::op(sum(r), T::inv(sum(l)));
  }

  void reset() {
    std::fill(t1.begin(), t1.end(), T::id());
    std::fill(t2.begin(), t2.end(), T::id());
  }

 private:
  static void add_one(std::vector<Info>& t, uint32_t n, uint32_t x,
                      const Info& v) {
    for (; x <= n; x += x & -x) {
      t[x] = T::op(t[x], v);
    }
  }

  static Info sum_one(const std::vector<Info>& t, uint32_t x) {
    Info res = T::id();
    for (; x > 0; x &= x - 1) {
      res = T::op(res, t[x]);
    }
    return res;
  }
};
