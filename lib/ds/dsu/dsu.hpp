#pragma once

#include "lib/utils/debug.hpp"

struct DSU {
  int n = 0;
  std::vector<int> fa;

  DSU() = default;
  explicit constexpr DSU(int _n) { resize(_n); }

  constexpr void resize(int _n) {
    CHECK(_n >= 0);
    n = _n;
    fa.assign(n, -1);
  }

  constexpr int find(int x) {
    CHECK(0 <= x && x < n);
    return fa[x] < 0 ? x : fa[x] = find(fa[x]);
  }

  constexpr int size(int x) {
    CHECK(0 <= x && x < n);
    x = find(x);
    return -fa[x];
  }

  constexpr bool merge(int x, int y) {
    CHECK(0 <= x && x < n);
    CHECK(0 <= y && y < n);
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

  constexpr bool same(int x, int y) {
    CHECK(0 <= x && x < n);
    CHECK(0 <= y && y < n);
    return find(x) == find(y);
  }
};
