// https://judge.yosupo.jp/problem/matrix_det

#pragma GCC optimize("Ofast,unroll-loops")
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "lib/utils/fast_io.hpp"
#include "lib/math/linalg/det_fixed_mod.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  io >> n;
  std::vector<std::vector<uint64_t>> mat(n, std::vector<uint64_t>(n));
  for (auto& x : mat) {
    for (auto& y : x) {
      y = io.in->read<uint32_t>();
    }
  }
  io << det_inplace<998244353>(mat, n);
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
