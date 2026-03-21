#pragma once

#include "lib/utils/debug.hpp"

// Based on https://codeforces.com/blog/entry/135239
template <bool weighted = false, typename T = int>
struct XorLinkedTree {
  uint32_t n = 0;
  std::vector<uint32_t> deg;
  std::vector<uint32_t> link;
  std::vector<T> val;

  XorLinkedTree() = default;
  explicit XorLinkedTree(uint32_t _n) { init(_n); };

  void init(uint32_t _n) {
    n = _n;
    deg.assign(n, 0);
    link.assign(n, 0);
    if constexpr (weighted) val.assign(n, 0);
  }

  void add_edge(uint32_t u, uint32_t v) requires(!weighted) {
    CHECK(u < n && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v;
    ++deg[v], link[v] ^= u;
  }

  void add_edge(uint32_t u, uint32_t v, T w) requires(weighted) {
    CHECK(u < n && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v, val[u] ^= w;
    ++deg[v], link[v] ^= u, val[v] ^= w;
  }

  template <typename F>
  void build(uint32_t root, F&& func) {
    CHECK(root < n);
    ++deg[root];

    for (uint32_t i = 0; i < n; ++i) {
      uint32_t u = i;
      while (deg[u] == 1) {
        uint32_t v = link[u];
        if constexpr (weighted) {
          T w = val[u];
          func(u, v, w);
          val[v] ^= w;
        } else {
          func(u, v);
        }
        deg[u] = 0;
        --deg[v], link[v] ^= u;
        u = v;
      }
    }
  }
};