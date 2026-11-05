// https://judge.yosupo.jp/problem/bitwise_and_convolution

#pragma GCC optimize(3)
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "lib/utils/fast_io.hpp"
#include "lib/math/conv/fwt_and.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr uint32_t P = 998244353;

void solve_main() {
  int n, m;
  io >> n;
  m = 1 << n;

  std::vector<uint32_t> a(m), b(m);
  for (auto& x : a) io >> x;
  for (auto& x : b) io >> x;

  fwt_and<P>(a.data(), m);
  fwt_and<P>(b.data(), m);
  for (int i = 0; i < m; ++i) {
    a[i] = 1ull * a[i] * b[i] % P;
  }
  ifwt_and<P>(a.data(), m);

  for (auto& x : a) {
    io << x << ' ';
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
