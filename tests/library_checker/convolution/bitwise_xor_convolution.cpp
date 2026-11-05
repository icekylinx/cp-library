// https://judge.yosupo.jp/problem/bitwise_xor_convolution

#pragma GCC optimize(3)
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "lib/utils/fast_io.hpp"
#include "lib/math/conv/fwt_xor.hpp"

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

  fwt_xor<P>(a.data(), m);
  fwt_xor<P>(b.data(), m);
  for (int i = 0; i < m; ++i) {
    a[i] = 1ull * a[i] * b[i] % P;
  }
  fwt_xor<P>(a.data(), m);

  auto qpow = [&](uint32_t x, uint32_t y, uint32_t k = 1) -> uint32_t {
    for (; y; y >>= 1, x = 1ull * x * x % P) {
      if (y & 1) k = 1ull * k * x % P;
    }
    return k;
  };

  uint32_t inv = qpow(m, P - 2);
  for (auto& x : a) {
    x = 1ull * x * inv % P;
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
