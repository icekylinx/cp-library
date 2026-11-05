// https://judge.yosupo.jp/problem/longest_increasing_subsequence

#pragma GCC optimize(3)
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iterator>
#include <vector>
#include "lib/utils/fast_io.hpp"
#include "lib/algo/fast_bounds.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  uint32_t n;
  io >> n;

  std::vector<uint32_t> idx(n), a(n);
  uint32_t r = 0;

  for (uint32_t i = 0; i < n; ++i) {
    uint32_t v = io.in->read<uint32_t>();
    if (!r || a[r - 1] < v) {
      a[r] = v;
      idx[i] = ++r;
    } else {
      auto pos = fast_lower_bound(a.begin(), r, v, std::less<uint32_t>());
      *pos = v;
      idx[i] = std::distance(a.begin(), pos) + 1;
    }
  }

  std::vector<uint32_t> res(r);
  for (uint32_t i = n; i--; ) {
    if (idx[i] == r) {
      res[--r] = i;
    }
  }

  io << res.size() << '\n';
  for (uint32_t x : res) {
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
