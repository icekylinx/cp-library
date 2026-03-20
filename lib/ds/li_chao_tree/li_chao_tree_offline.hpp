#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct LiChaoTreeOffline {
  using Line  = typename T::Line;
  using Value = typename T::Value;
  using Range = typename T::Range;

  uint32_t n = 0;
  uint32_t m = 0;
  std::vector<Line> t;
  std::vector<Range> idx;
  std::vector<Value> val;

  template <typename It>
  LiChaoTreeOffline(It first, It last) { build(first, last); }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    idx.assign(n + 1, T::range_id());
    val.assign(n, T::evaluate(T::id(), T::range_id()));
    std::copy(first, last, idx.begin());
  }

  void add(Line x) {
    uint32_t k = 1, l = 0, r = n;
    Value xl = T::evaluate(x, idx[l]), xr = T::evaluate(x, idx[r]);

    while (true) {
      if (l + 1 == r) {
        val[l] = T::op(xl, val[l]);
        return;
      }

      Line& y = t[k];
      Value yl = T::evaluate(y, idx[l]), yr = T::evaluate(y, idx[r]);
      uint32_t mid = (l + r) >> 1;
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

  Value get(uint32_t x) const {
    CHECK(x < m);
    Range v = idx[x];
    Value res = val[x];
    x += n;
    while (x >>= 1) res = T::op(res, T::evaluate(t[x], v));
    return res;
  }
};