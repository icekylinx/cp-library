// https://judge.yosupo.jp/problem/unionfind

#include <cassert>
#include <cstdint>
#include <cstdio>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/dsu/dsu.hpp"

using ll = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n, q;
  io >> n >> q;

  DSU dsu(n);

  while (q--) {
    bool op = io.in->read<bool>();
    if (op == 0) {
      dsu.merge(io.in->read_small<uint32_t>(), io.in->read_small<uint32_t>());
    } else {
      io << dsu.same(io.in->read_small<uint32_t>(),
                     io.in->read_small<uint32_t>())
         << '\n';
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
