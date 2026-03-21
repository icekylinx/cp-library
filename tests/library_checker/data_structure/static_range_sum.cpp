// https://judge.yosupo.jp/problem/static_range_sum

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io_basic.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

constexpr int N = 5e5 + 5;

FastIO<1 << 20, 1 << 19> io;

uint64_t sum[N];

void solve_main() {
  int n, q;
  io >> n >> q;

  for (int i = 1; i <= n; ++i) {
    sum[i] = io.in->read<uint32_t>();
  }

  partial_sum(sum + 1, sum + n + 1, sum + 1);

  while (q--) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    io << sum[r] - sum[l] << '\n';
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