// https://judge.yosupo.jp/problem/static_range_frequency

#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/hash_map/fast_hash_map.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

FastHashMap<uint32_t, uint32_t, 19> mp;

void solve_main() {
  uint32_t n, q;
  io >> n >> q;

  vector<uint32_t> ans(q), a(n), cnt(n + 1, 1);
  vector<pair<uint32_t, uint32_t>> aux(n + (q << 1));
  vector<tuple<uint32_t, uint32_t, uint32_t>> qry(q);

  for (uint32_t& x : a) {
    x = io.in->read<uint32_t>();
  }

  for (uint32_t i = 0; i < q; ++i) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    ++cnt[l], ++cnt[r];
    qry[i] = make_tuple(l, r, io.in->read<uint32_t>());
  }

  for (uint32_t i = 0; i < n; ++i) {
    cnt[i + 1] += cnt[i];
  }
  --cnt[n];
  
  for (uint32_t i = 0; i < n; ++i) {
    aux[--cnt[i]] = make_pair(-1u, a[i]);
  }

  for (uint32_t i = 0; i < q; ++i) {
    auto [l, r, x] = qry[i];
    aux[--cnt[l]] = make_pair(i << 1, x);
    aux[--cnt[r]] = make_pair(i << 1 | 1, x);
  }

  for (const auto& [id, val] : aux) {
    if (~id == 0) {
      ++mp[val];
    } else {
      ans[id >> 1] += ((id & 1) * 2 - 1) * mp.get(val);
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