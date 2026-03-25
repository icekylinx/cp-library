// https://judge.yosupo.jp/problem/tree_diameter

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/tree/xor_linked_tree.hpp"

using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  io >> n;
  XorLinkedTree<true, uint32_t> t(n);

  for (int i = 0; i < n - 1; ++i) {
    uint32_t u = io.in->read_small<uint32_t>();
    uint32_t v = io.in->read_small<uint32_t>();
    uint32_t w = io.in->read<uint32_t>();
    t.add_edge(u, v, w);
  }

  vector<uint64_t> mx(n, 0);
  vector<uint32_t> fa(n, 0);
  auto pos = views::iota(0, n) | ranges::to<vector<uint32_t>>();

  uint64_t ans = 0;
  uint32_t l = 0, r = 0, lca = 0;

  t.build(0, [&](uint32_t u, uint32_t v, uint32_t w) -> void {
    if (u == 0) return;
    fa[u] = v;

    uint64_t new_mx = mx[u] + w;
    uint64_t new_ans = new_mx + mx[v];

    if (new_ans > ans) {
      ans = new_ans;
      l = pos[u], r = pos[v], lca = v;
    }

    if (new_mx > mx[v]) {
      mx[v] = new_mx;
      pos[v] = pos[u];
    }
  });

  uint32_t cnt = 0;

  while (l != lca) {
    pos[cnt] = l, ++cnt;
    l = fa[l];
  }

  pos[cnt] = lca;
  uint32_t mid = ++cnt;

  while (r != lca) {
    pos[cnt] = r, ++cnt;
    r = fa[r];
  }

  io << ans << ' ' << cnt << '\n';

  for (int i = 0; i < mid; ++i) io << pos[i] << ' ';
  for (int i = cnt - 1; i >= mid; --i) io << pos[i] << ' ';
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