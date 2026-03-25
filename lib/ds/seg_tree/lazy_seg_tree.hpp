#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct LazySegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  uint32_t n = 0;
  uint32_t m = 0;
  int h = 0;
  std::vector<Info> t;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  LazySegTree() = default;
  explicit LazySegTree(uint32_t _m) { build(_m); }

  template <typename F>
  LazySegTree(uint32_t _m, F&& func) { build(_m, func); }

  template <typename It>
  LazySegTree(It first, It last) { build(first, last); }

  void build(uint32_t _m) {
    m = _m;
    n = 1;
    while (n < m) n <<= 1;
    h = std::bit_width(n) - 1;
    t.assign(n << 1, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
  }

  template <typename F>
  void build(uint32_t _m, F&& func) {
    build(_m);
    for (uint32_t i = 0; i < m; ++i) t[n + i] = func(i);
    for (uint32_t i = n - 1; i > 0; --i) push_up(i);
  }

  template <typename It>
  void build(It first, It last) {
    build(static_cast<uint32_t>(std::distance(first, last)));
    std::copy(first, last, t.begin() + n);
    for (uint32_t i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
    std::fill(tag.begin(), tag.end(), T::tag_id());
    std::fill(has_tag.begin(), has_tag.end(), 0);
  }

  void set(uint32_t x, const Info& v) {
    CHECK(x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(uint32_t x) {
    CHECK(x < m);
    x += n;
    Info res = t[x];
    while (x >>= 1) T::apply(res, tag[x]);
    return res;
  }

  void apply(uint32_t l, uint32_t r, const Tag& v) {
    CHECK(l <= r && r <= m);
    if (l == r) return;
    l += n, r += n;
    int w = std::bit_width(l ^ r) - 1;
    int cl = std::countr_zero(l);
    int cr = std::countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i);
    for (int i = std::max(cl, w); i > cr; --i) push_down(r >> i);

    uint32_t L = l, R = r;
    int k = std::bit_width(--L ^ R) - 1;

    uint32_t cL = L;
    L = ~L & ((1 << k) - 1);
    while (L > 0) {
      int i = std::countr_zero(L);
      L ^= 1 << i;
      apply_node(cL >> i ^ 1, v);
    }

    uint32_t cR = R;
    R &= (1 << k) - 1;
    while (R > 0) {
      int i = std::bit_width(R) - 1;
      R ^= 1 << i;
      apply_node(cR >> i ^ 1, v);
    }

    for (int i = cl + 1; i <= w; ++i) push_up(l >> i);
    for (int i = cr + 1; i <= w; ++i) push_up(r >> i);
    for (l >>= w + 1; l > 0; l >>= 1) push_up(l);
  }

  void apply_all(const Tag& v) { apply_node(1, v); }

  Info prod(uint32_t l, uint32_t r) {
    CHECK(l <= r && r <= m);
    if (l == r) return T::id();
    Info left = T::id(), right = T::id();
    l += n - 1, r += n;
    while ((l ^ r) != 1) {
      if (~l & 1) left = T::op(left, t[l ^ 1]);
      if (r & 1) right = T::op(t[r ^ 1], right);
      l >>= 1, r >>= 1;
      T::apply(left, tag[l]);
      T::apply(right, tag[r]);
    }

    Info sum = T::op(left, right);
    while (l >>= 1) T::apply(sum, tag[l]);
    return sum;
  }

  Info all_prod() const { return t[1]; }

 private:
  void push_up(uint32_t x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }

  void apply_node(uint32_t x, const Tag& v) {
    has_tag[x] = 1;
    T::apply(t[x], v);
    T::compose(tag[x], v);
  }

  void push_down(uint32_t x) {
    if (has_tag[x]) {
      apply_node(x << 1, tag[x]);
      apply_node(x << 1 | 1, tag[x]);
      tag[x] = T::tag_id();
      has_tag[x] = 0;
    }
  }
};