#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct LiChaoSegTreeOffline {
  using Line  = typename T::Line;
  using Value = typename T::Value;
  using Range = typename T::Range;

  uint32_t n = 0;
  uint32_t m = 0;
  std::vector<Line> t;
  std::vector<Range> idx;
  std::vector<Value> val;

  template <typename It>
  LiChaoSegTreeOffline(It first, It last) { build(first, last); }

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

  void add_seg(uint32_t l, uint32_t r, const Line& line) {
    CHECK(l <= r && r <= m);
    if (l == r) [[unlikely]] return;
    l += n - 1, r += n;
    int w = std::bit_width(l ^ r) - 1;

    uint32_t cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = std::countr_zero(l);
      l &= l - 1;
      add(cl >> i ^ 1, i, line);
    }

    uint32_t cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = std::countr_zero(r);
      r &= r - 1;
      add(cr >> i ^ 1, i, line);
    }
  }

  void add_line(const Line& line) {
    add(1, std::bit_width(n) - 1, line);
  }

  Value get(uint32_t x) const {
    CHECK(x < m);
    Range v = idx[x];
    Value res = val[x];
    x += n;
    while (x >>= 1) res = T::op(res, T::evaluate(t[x], v));
    return res;
  }

 private:
  void add(uint32_t k, int i, Line x) {
    uint32_t l = (k << i) ^ n;
    uint32_t r = l + (1 << i);
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
};