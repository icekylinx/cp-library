// https://judge.yosupo.jp/problem/point_set_range_composite

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <utility>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/seg_tree.hpp"

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int P = 998244353;

struct PointSetRangeComposite {
  using Info = std::pair<uint32_t, uint32_t>;
  static constexpr Info id() noexcept { return {1, 0}; };

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {1ull * lhs.first * rhs.first % P,
            (1ull * lhs.second * rhs.first + rhs.second) % P};
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  SegTree<PointSetRangeComposite> t(n, [&](int i) {
    uint32_t a, b;
    io >> a >> b;
    return std::make_pair(a, b);
  });

  auto eval = [&](uint32_t x, const auto& info) -> uint32_t {
    return (1ull * x * info.first + info.second) % P;
  };

  while (q--) {
    bool op;
    uint32_t a, b, c;
    io >> op;
    if (op == 0) {
      a = io.in->read_small<uint32_t>();
      io >> b >> c;
      t.set(a, {b, c});
    } else {
      a = io.in->read_small<uint32_t>();
      b = io.in->read_small<uint32_t>();
      io >> c;
      io << t.evaluate(a, b, c, eval) << '\n';
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
