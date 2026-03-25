// https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/math/linalg/det_mod.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  uint32_t P;
  io >> n >> P;
  vector<vector<uint32_t>> mat(n, vector<uint32_t>(n));
  for (auto& x : mat) {
    for (auto& y : x) {
      io >> y;
    }
  }
  io << det_inplace(mat, n, P);
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