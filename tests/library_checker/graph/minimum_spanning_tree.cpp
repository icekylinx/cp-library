// https://judge.yosupo.jp/problem/minimum_spanning_tree

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/dsu/dsu.hpp"
#include "lib/algo/radix_sort.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n, m;
  io >> n >> m;

  vector<pair<uint32_t, uint32_t>> edge(m);
  vector<uint64_t> id(m);
  vector<uint32_t> mst(n - 1);

  for (int i = 0; i < m; ++i) {
    auto& [a, b] = edge[i];
    a = io.in->read_small<uint32_t>();
    b = io.in->read_small<uint32_t>();
    uint32_t c = io.in->read<uint32_t>();
    id[i] = static_cast<uint64_t>(i) << 32 | c;
  }

  radix_sort_u32<uint64_t, 15>(id.data(), m, [&](auto x) {
    return static_cast<uint32_t>(x);
  });

  DSU dsu(n);
  int cnt = 0;
  uint64_t ans = 0;
  for (auto x : id) {
    uint32_t i = static_cast<uint32_t>(x >> 32);
    if (dsu.merge(edge[i].first, edge[i].second)) {
      ans += static_cast<uint32_t>(x);
      mst[cnt++] = i;
    }
    if (cnt == n - 1) break;
  }

  io << ans << '\n';
  for (auto x : mst) io << x << ' ';
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