#pragma once

#include "lib/utils/debug.hpp"

struct DSU {
  uint32_t n = 0;
  std::vector<int> fa;

  DSU() = default;
  explicit constexpr DSU(uint32_t _n) { resize(_n); }

  constexpr void resize(uint32_t _n) {
    n = _n;
    fa.assign(n, -1);
  }

  constexpr uint32_t find(uint32_t x) {
    CHECK(x < n);
    return fa[x] < 0 ? x : fa[x] = find(fa[x]);
  }

  constexpr uint32_t size(uint32_t x) {
    CHECK(x < n);
    return -fa[find(x)];
  }

  constexpr bool merge(uint32_t x, uint32_t y) {
    CHECK(x < n && y < n);
    x = find(x);
    y = find(y);
    if (x == y) return false;
    if (fa[x] < fa[y]) {
      fa[x] += fa[y], fa[y] = x;
    } else {
      fa[y] += fa[x], fa[x] = y;
    }
    return true;
  }

  constexpr bool same(uint32_t x, uint32_t y) {
    CHECK(x < n && y < n);
    return find(x) == find(y);
  }
};
