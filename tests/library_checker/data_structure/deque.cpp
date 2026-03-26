// https://judge.yosupo.jp/problem/deque

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int N = 5e5 + 5;

uint32_t pool[N << 1];
uint32_t* l = pool + N;
uint32_t* r = pool + N - 1;

void solve_main() {
  int q;
  io >> q;

  while (q--) {
    char op = io.in->read<char>();
    if (op == '0') {
      *--l = io.in->read<uint32_t>();
    } else if (op == '1') {
      *++r = io.in->read<uint32_t>();
    } else if (op == '2') {
      ++l;
    } else if (op == '3') {
      --r;
    } else {
      io << l[io.in->read_small<uint32_t>()] << '\n';
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