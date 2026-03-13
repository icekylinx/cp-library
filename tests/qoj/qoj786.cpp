// https://qoj.ac/problem/786

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/string/z_algo.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

// FastIO<1 << 20, 1 << 19> io;
fast_io::FastOutput<1 << 19> io;

constexpr int N = 2e6 + 5;

char s[N];

void solve_main() {
  uint32_t n = fread(s, 1, N, stdin) - 1;
  auto z = z_algo(s, n);
  z[0] = 0;
  for (int x : z) {
    io << static_cast<uint32_t>(x) << ' ';
  }
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  // cin.tie(nullptr)->sync_with_stdio(false);
  // io.init();

  int T;
  // cin >> T;
  // io >> T;
  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}