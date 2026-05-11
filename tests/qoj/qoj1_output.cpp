// https://qoj.ac/problem/1

#include <bits/stdc++.h>
#include <cassert>
// #include "lib/utils/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

// FastIO<1 << 20, 1 << 19> io;

constexpr int N = 1e7;

char a[10 * N];

void solve_main() {
  int n;
  cin >> n;

  for (int i = 0; i < n; ++i) {
    a[i * 10 + 0] = '9';
    a[i * 10 + 1] = '9';
    a[i * 10 + 2] = '9';
    a[i * 10 + 3] = '9';
    a[i * 10 + 4] = '9';
    a[i * 10 + 5] = '9';
    a[i * 10 + 6] = '9';
    a[i * 10 + 7] = '9';
    a[i * 10 + 8] = '9';
    a[i * 10 + 9] = ' ';
  }

  fwrite(a, 1, n * 10, stdout);
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  cin.tie(nullptr)->sync_with_stdio(false);
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
