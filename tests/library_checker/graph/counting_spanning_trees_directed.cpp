// https://judge.yosupo.jp/problem/counting_spanning_tree_directed

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
  int n, m, r;
  io >> n >> m >> r;

  std::vector<std::vector<uint64_t>> mat(n, std::vector<uint64_t>(n, P));
  for (int i = 0; i < m; ++i) {
    uint32_t u = io.in->read_small<uint32_t>();
    uint32_t v = io.in->read_small<uint32_t>();
    --mat[u][v];
    ++mat[v][v];
  }

  for (int i = 0; i < n; ++i) {
    mat[i][r] = mat[r][i] = 0;
  }
  mat[r][r] = 1;

  io << det_inplace<998244353>(mat, n); 
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