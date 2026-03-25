// https://judge.yosupo.jp/problem/counting_spanning_tree_undirected

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/math/linalg/det_fixed_mod.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr uint32_t P = 998244353;

void solve_main() {
  int n, m;
  io >> n >> m;

  if (n == 1) [[unlikely]] {
    io << 1;
    return;
  }

  std::vector<std::vector<uint64_t>> mat(n, std::vector<uint64_t>(n, P));
  for (int i = 0; i < m; ++i) {
    uint32_t u = io.in->read_small<uint32_t>();
    uint32_t v = io.in->read_small<uint32_t>();
    --mat[u][v], --mat[v][u];
    ++mat[u][u], ++mat[v][v];
  }

  io << det_inplace<998244353>(mat, n - 1); 
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  // cin.tie(nullptr)->sync_with_stdio(false);
  io.init();

  int T;
  // cin >> T;
  // io >> T;
  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}