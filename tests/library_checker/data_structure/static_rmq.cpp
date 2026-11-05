// https://judge.yosupo.jp/problem/staticrmq

#pragma GCC optimize(3)
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <limits>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

template <typename T>
struct RangeMin {
  using Info = T;

  static constexpr Info id() { return std::numeric_limits<T>::max(); }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return std::min(lhs, rhs);
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  BlockedSparseTable<RangeMin<uint32_t>> t(n, [&](uint32_t i) {
    return io.in->read<uint32_t>();
  });

  while (q--) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    io << t.prod(l, r) << '\n';
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
