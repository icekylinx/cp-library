// https://judge.yosupo.jp/problem/zalgorithm

#include <cassert>
#include <cstdint>
#include <cstdio>
#include "lib/utils/fast_io.hpp"
#include "lib/string/z_algo.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  auto& cur = io.in->cur;
  uint32_t n = 0;
  while (cur[n] >= 'a') ++n;
  for (uint32_t x : z_algo(cur, n)) {
    io.out->write_w8(x);
    io << ' ';
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
