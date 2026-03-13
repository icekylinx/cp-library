// https://judge.yosupo.jp/problem/zalgorithm

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/string/z_algo.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  auto& cur = io.in->cur;
  uint32_t n = 0;
  while (cur[n] >= 'a') ++n;
  for (int x : z_algo(cur, n)) {
    io << static_cast<uint32_t>(x) << ' ';
  }
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