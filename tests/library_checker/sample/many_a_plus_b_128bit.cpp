// https://judge.yosupo.jp/problem/many_aplusb_128bit

#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
#include "lib/utils/fast_io_simd.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr uint32_t B = 1 << 12;

__int128_t sum[B];

void solve_main() {
  uint32_t n;
  io >> n;

  for (uint32_t base = 0; base < n; base += B) {
    uint32_t cnt = min(B, n - base);
    for (uint32_t i = 0; i < cnt; ++i) {
      sum[i] = io.in->read<__int128_t>() + io.in->read<__int128_t>();
    }
    for (uint32_t i = 0; i < cnt; ++i) {
      io << sum[i] << '\n';
    }
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