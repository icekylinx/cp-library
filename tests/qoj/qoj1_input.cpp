// https://qoj.ac/problem/1

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include <cassert>
#include "lib/utils/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  io >> n;

  uint64_t sum = 0;

  while (n--) {
    sum += io.in->read<uint32_t>();
  }

  io << sum;
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