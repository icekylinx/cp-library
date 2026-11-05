// https://judge.yosupo.jp/problem/range_add_range_min

#pragma GCC optimize(3)
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <utility>
#include <vector>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/seg_tree.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

struct PrefixMin {
  using Info = std::pair<int64_t, int64_t>;

  static constexpr int64_t inf = 1e18;
  static constexpr Info id() { return std::make_pair(0, inf); }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return std::make_pair(lhs.first + rhs.first,
                     std::min(lhs.second, lhs.first + rhs.second));
  }
};

void solve_main() {
  uint32_t n, q;
  io >> n >> q;

  std::vector<int64_t> a(n);

  int32_t lst = 0;
  for (auto& x : a) {
    int32_t v;
    io >> v;
    x = v - lst;
    lst = v;
  }

  SegTree<PrefixMin> t(n, [&](int i) {
    return std::make_pair(a[i], a[i]);
  });

  while (q--) {
    bool op = io.in->read<bool>();
    uint32_t l = io.in->read<uint32_t>();
    uint32_t r = io.in->read<uint32_t>();
    if (op == 0) {
      int32_t x;
      io >> x;
      a[l] += x;
      t.set(l, std::make_pair(a[l], a[l]));
      if (r < n) [[likely]] {
        a[r] -= x;
        t.set(r, std::make_pair(a[r], a[r]));
      }
    } else {
      io << t.evaluate(0, l, 0ll, [&](int64_t a, const auto& x) {
        return a + x.first;
      }) + t.prod(l, r).second << '\n';
    }
  }
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  io.init();
  solve_main();
  return 0;
}
