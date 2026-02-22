#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct FenwickTreeRange {
  int n = 0;
  std::vector<T> t1, t2;

  FenwickTreeRange() = default;
  explicit FenwickTreeRange(int _n) { resize(_n); }

  void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    t1.assign(n + 1, T::id());
    t2.assign(n + 1, T::id());
  }

  static void add_one(std::vector<T>& t, int n, int x, const T& v) {
    for (; x <= n; x += x & -x) {
      t[x] = T::op(t[x], v);
    }
  }

  static T sum_one(const std::vector<T>& t, int x) {
    T res = T::id();
    for (; x > 0; x &= x - 1) {
      res = T::op(res, t[x]);
    }
    return res;
  }

  void add(int l, int r, const T& v) {
    CHECK(0 <= l && l <= r && r <= n);
    ++l; ++r;
    add_one(t1, n, l, v);
    add_one(t1, n, r, T::inv(v));
    add_one(t2, n, l, T::pow(T::inv(v), l - 1));
    add_one(t2, n, r, T::pow(v, r - 1));
  }

  T sum(int x) const {
    CHECK(0 <= x && x <= n);
    T s1 = sum_one(t1, x);
    T s2 = sum_one(t2, x);
    return T::op(T::pow(s1, x), s2);
  }

  T sum(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= n);
    return T::op(T::inv(sum(l)), sum(r));
  }

  void reset() {
    std::fill(t1.begin(), t1.end(), T::id());
    std::fill(t2.begin(), t2.end(), T::id());
  }
};
