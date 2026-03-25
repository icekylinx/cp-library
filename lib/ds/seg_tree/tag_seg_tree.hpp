#pragma once

#include "lib/utils/debug.hpp"

template <typename T>
struct TagSegTree {
  using Info = typename T::Info;
  using Tag  = typename T::Tag;

  uint32_t n = 0;
  uint32_t m = 0;
  int h = 0;
  std::vector<Info> val;
  std::vector<Tag> tag;
  std::vector<uint8_t> has_tag;

  TagSegTree() = default;
  explicit TagSegTree(uint32_t _m) { build(_m); }

  template <typename F>
  TagSegTree(uint32_t _m, F&& func) { build(_m, func); }

  template <typename It>
  TagSegTree(It first, It last) { build(first, last); }

  void build(uint32_t _m) {
    m = _m;
    n = 1;
    while (n < m) n <<= 1;
    h = std::bit_width(n) - 1;
    val.assign(n, T::id());
    tag.assign(n << 1, T::tag_id());
    has_tag.assign(n << 1, 0);
  }

  template <typename F>
  void build(uint32_t _m, F&& func) {
    build(_m);
    for (uint32_t i = 0; i < m; ++i) val[i] = func(i);
  }

  template <typename It>
  void build(It first, It last) {
    build(static_cast<uint32_t>(std::distance(first, last)));
    std::copy(first, last, val.begin());
  }

  void reset() {
    std::fill(val.begin(), val.end(), T::id());
    std::fill(tag.begin(), tag.end(), T::tag_id());
    std::fill(has_tag.begin(), has_tag.end(), 0);
  }

  void set(uint32_t x, const Info& v) {
    CHECK(x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    tag[x] = T::tag_id();
    val[x - n] = v;
  }

  Info get(uint32_t x) const {
    CHECK(x < m);
    Info res = val[x];
    x += n;
    for (int i = 0; i <= h; ++i) T::apply(res, tag[x >> i]);
    return res;
  }

  Info& get_ref(uint32_t x) {
    CHECK(x < m);
    x += n;
    for (int i = h; i > 0; --i) push_down(x >> i);
    T::apply(val[x - n], tag[x]);
    tag[x] = T::tag_id();
    return val[x - n];
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

    w = std::bit_width(--l ^ r) - 1;

    cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = std::countr_zero(l);
      l ^= 1 << i;
      apply_node(cl >> i ^ 1, v);
    }

    cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = std::bit_width(r) - 1;
      r ^= 1 << i;
      apply_node(cr >> i ^ 1, v);
    }
  }

 private:
  void apply_node(uint32_t x, const Tag& v) {
    has_tag[x] = 1;
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