// https://judge.yosupo.jp/problem/static_range_count_distinct

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/hash_map/fast_hash_map.hpp"
#include "lib/ds/fenwick_tree/fast_fenwick_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

FastHashMap<uint32_t, uint32_t, 19> mp;

struct Sum {
  using Info = uint32_t;

  static constexpr Info id() { return 0; }
  static constexpr Info op(uint32_t x, uint32_t y) { return x + y; }
  static constexpr Info inv(uint32_t x) { return -x; }
};

void solve_main() {
  uint32_t n, q;
  io >> n >> q;

  vector<uint32_t> pre(n + 1), ans(q), cnt(n + 1, 1);
  vector<pair<uint32_t, uint32_t>> aux(n + q), qry(q);

  cnt[0] = 0;
  for (uint32_t i = 1; i <= n; ++i) {
    pre[i] = exchange(mp[io.in->read<uint32_t>()], i);
  }

  io.in->skip_space();

  for (uint32_t i = 0; i < q; ++i) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    ++cnt[r];
    ans[i] = -l;
    qry[i] = make_pair(l, r);
  }

  for (uint32_t i = 0; i < n; ++i) {
    cnt[i + 1] += cnt[i];
  }
  
  for (uint32_t i = 0; i < q; ++i) {
    auto [l, r] = qry[i];
    aux[--cnt[r]] = make_pair(i, l + 1);
  }

  for (uint32_t i = 1; i <= n; ++i) {
    aux[--cnt[i]] = make_pair(-1u, pre[i]);
  }

  FastFenwickTree<Sum> t(n + 1);

  for (const auto& [id, val] : aux) {
    if (~id == 0) {
      t.add(val, 1);
    } else {
      ans[id] += t.sum(val);
    }
  }

  for (uint32_t i = 0; i < q; ++i) {
    io.out->write_w8(ans[i]);
    io << '\n';
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