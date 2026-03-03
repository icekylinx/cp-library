// https://judge.yosupo.jp/problem/range_affine_point_get

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/seg_tree/tag_seg_tree.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int P = 998244353;

struct RangeAffinePointGet {
  using Info = uint32_t;

  struct Tag {
    uint32_t b, c;
  };

  static constexpr Info id() noexcept { return 0; };
  static constexpr Tag tag_id() noexcept { return {1, 0}; };

  static constexpr void compose(Tag& lhs, const Tag& rhs) {
    lhs.b = static_cast<uint64_t>(lhs.b) * rhs.b % P;
    lhs.c = (static_cast<uint64_t>(lhs.c) * rhs.b + rhs.c) % P;
  }

  static constexpr void apply(Info& info, const Tag& tag) {
    info = (static_cast<uint64_t>(tag.b) * info + tag.c) % P;
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  TagSegTree<RangeAffinePointGet> t(n, [&](int i) {
    return io.in->read<uint32_t>();
  });

  while (q--) {
    bool op;
    io >> op;
    if (op == 0) {
      uint32_t l = io.in->read_small<uint32_t>();
      uint32_t r = io.in->read_small<uint32_t>();
      uint32_t b = io.in->read<uint32_t>();
      uint32_t c = io.in->read<uint32_t>();
      t.apply(l, r, {b, c});
    } else {
      io << t.get(io.in->read_small<uint32_t>()) << '\n';
    }
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