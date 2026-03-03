// https://judge.yosupo.jp/problem/point_add_range_sum

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/fenwick_tree/fast_fenwick_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int N = 5e5 + 5;

struct Node {
  using Info = uint64_t;

  static constexpr Info id() { return 0; }
  static constexpr Info op(uint64_t x, uint64_t y) { return x + y; }
  static constexpr Info inv(uint64_t x) { return -x; }
};

uint64_t a[N];

void solve_main() {
  int n, q;
  io >> n >> q;

  FastFenwickTree<Node> t(n);

  for (int i = 1; i <= n; ++i) {
    a[i] = io.in->read<uint32_t>();
  }

  partial_sum(a + 1, a + n + 1, a + 1);

  while (q--) {
    bool op = io.in->read<bool>();
    uint32_t x = io.in->read_small<uint32_t>();
    if (op == 0) {
      t.add(x, io.in->read<uint32_t>());
    } else {
      uint32_t y = io.in->read_small<uint32_t>();
      io << t.sum(x, y) + a[y] - a[x] << '\n';
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