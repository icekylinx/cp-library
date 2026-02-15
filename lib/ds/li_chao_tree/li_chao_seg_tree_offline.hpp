#pragma once

#include "lib/utils/debug.hpp"
#include "lib/math/my_bit.hpp"

template <typename T>
struct LiChaoSegTreeOffline {
  using Line  = typename T::Line;
  using Value = typename T::Value;
  using Range = typename T::Range;

  int n = 0;
  int m = 0;
  int h = 0;
  std::vector<Line> t;
  std::vector<Range> idx;
  std::vector<Value> val;

  template <typename It>
  LiChaoSegTreeOffline(It first, It last) { build(first, last); }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = internal::__lg(n);
    t.assign(n << 1, T::id());
    idx.assign(n + 1, T::range_id());
    val.assign(n, T::evaluate(T::id(), T::range_id()));
    std::copy(first, last, idx.begin());
  }

  void add_seg(int l, int r, const Line& line) {
    CHECK(0 <= l && l <= r && r <= m);
    if (l == r) [[unlikely]] return;
    l += n - 1, r += n;
    int w = internal::__lg(l ^ r);

    int cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = internal::countr_zero(l);
      l &= l - 1;
      add(cl >> i ^ 1, i, line);
    }

    int cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = internal::countr_zero(r);
      r &= r - 1;
      add(cr >> i ^ 1, i, line);
    }
  }

  void add_line(const Line& line) {
    add(1, h, line);
  }

  Value get(int x) const {
    CHECK(0 <= x && x < m);
    Range v = idx[x];
    Value res = val[x];
    x += n;
    while (x >>= 1) res = T::op(res, T::evaluate(t[x], v));
    return res;
  }

 private:
  void add(int k, int i, Line x) {
    int l = (k << i) ^ n;
    int r = l + (1 << i);
    Value xl = T::evaluate(x, idx[l]), xr = T::evaluate(x, idx[r]);

    while (true) {
      if (l + 1 == r) {
        val[l] = T::op(xl, val[l]);
        return;
      }

      Line& y = t[k];
      Value yl = T::evaluate(y, idx[l]), yr = T::evaluate(y, idx[r]);
      int mid = (l + r) >> 1;
      if (T::compare(xl, yl)) {
        if (T::compare(xr, yr)) {
          std::swap(x, y);
          return;
        } else {
          Value xmid = T::evaluate(x, idx[mid]);
          Value ymid = T::evaluate(y, idx[mid]);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k = k << 1 | 1, xl = ymid, xr = yr, l = mid;
          } else {
            k <<= 1, xr = xmid, r = mid;
          }
        }
      } else {
        if (T::compare(xr, yr)) {
          Value xmid = T::evaluate(x, idx[mid]);
          Value ymid = T::evaluate(y, idx[mid]);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k <<= 1, xl = yl, xr = ymid, r = mid;
          } else {
            k = k << 1 | 1, xl = xmid, l = mid;
          }
        } else {
          return;
        }
      }
    }
  }
};