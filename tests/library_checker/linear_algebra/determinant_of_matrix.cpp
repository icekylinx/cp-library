// https://judge.yosupo.jp/problem/matrix_det

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/fast_io_basic.hpp"
#include "lib/math/linalg/det_fixed_mod.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  io >> n;
  vector<vector<uint64_t>> mat(n, vector<uint64_t>(n));
  for (auto& x : mat) {
    for (auto& y : x) {
      y = io.in->read<uint32_t>();
    }
  }
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