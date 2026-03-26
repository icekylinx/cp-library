// https://judge.yosupo.jp/problem/lca

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/tree/parent_to_dfn.hpp"
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

template <typename T>
struct RangeMin {
  using Info = T;

  static constexpr Info id() { return numeric_limits<T>::max(); }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return min(lhs, rhs);
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  vector<uint32_t> p(n), rev(n);
  for (int i = 1; i < n; ++i) {
    p[i] = io.in->read_small<uint32_t>();
  }
  auto dfn = parent_to_dfn(p);
  for (int i = 1; i < n; ++i) {
    rev[dfn[i]] = p[i];
  }

  BlockedSparseTable<RangeMin<uint32_t>, 64> t(n, [&](uint32_t i) {
    return rev[i];
  });

  while (q--) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    uint32_t u = std::min(dfn[l], dfn[r]);
    uint32_t v = std::max(dfn[l], dfn[r]);
    io << t.prod(u + 1, v + 1) << '\n';
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