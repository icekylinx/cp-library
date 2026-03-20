#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct SegTree {
  using Info = typename T::Info;

  uint32_t n = 0;
  uint32_t m = 0;
  std::vector<Info> t;

  SegTree() = default;
  explicit SegTree(uint32_t _m) { build(_m); }

  template <typename F>
  SegTree(uint32_t _m, F&& func) { build(_m, func); }

  template <typename It>
  SegTree(It first, It last) { build(first, last); }

  void build(uint32_t _m) {
    m = _m;
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
  }

  template <typename F>
  void build(uint32_t _m, F&& func) {
    m = _m;
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    for (uint32_t i = 0; i < m; ++i) t[n + i] = func(i);
    for (uint32_t i = n - 1; i > 0; --i) push_up(i);
  }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    std::copy(first, last, t.begin() + n);
    for (uint32_t i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
  }

  void set(uint32_t x, const Info& v) {
    CHECK(x < m);
    x += n;
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(uint32_t x) const {
    CHECK(x < m);
    return t[n + x];
  }

  Info prod(uint32_t l, uint32_t r) const {
    CHECK(l <= r && r <= m);
    return evaluate(l, r, T::id(), [&](const Info& x, const Info& y) {
      return T::op(x, y);
    });
  }

  Info all_prod() const { return t[1]; }

  template <typename Value, typename F>
  Value evaluate(uint32_t l, uint32_t r, Value val, F&& func) const {
    CHECK(l <= r && r <= m);
    l += n - 1, r += n;
    int w = std::bit_width(l ^ r) - 1;

    uint32_t cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = std::countr_zero(l);
      l ^= 1 << i;
      val = func(val, t[cl >> i ^ 1]);
    }

    uint32_t cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = std::bit_width(r) - 1;
      r ^= 1 << i;
      val = func(val, t[cr >> i ^ 1]);
    }

    return val;
  }

 private:
  void push_up(uint32_t x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }
};