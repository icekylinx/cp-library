// https://judge.yosupo.jp/problem/segment_add_get_min

#include <bits/stdc++.h>
#include "lib/utils/fast_io_basic.hpp"
#include "lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp"
#include "lib/algo/radix_sort.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int64_t inf = 3e18;

struct Node {
  using Line  = pair<int32_t, int64_t>;
  using Value = int64_t;
  using Range = int32_t;

  static constexpr Line id() { return make_pair(0, inf); }

  static constexpr Range range_id() { return 1e9 + 5; }

  static constexpr Value evaluate(const Line& line, Range x) {
    return static_cast<Value>(line.first) * x + line.second;
  }

  static constexpr bool compare(Value x, Value y) { return x < y; }

  static constexpr Value op(Value x, Value y) { return min(x, y); }
};

void solve_main() {
  int n, q;
  io >> n >> q;
  vector<tuple<int, int, int, int64_t>> seg;
  seg.resize(n + q);
  vector<int> points;
  vector<uint64_t> aux;
  points.reserve(q);
  aux.reserve((n + q) << 1);

  uint32_t offset = 1u << 31;

  auto compress = [&](int i, int32_t v) -> uint64_t {
    uint32_t tmp = static_cast<uint32_t>(v) + offset;
    return static_cast<uint64_t>(i) << 32 | tmp;
  };

  for (int i = 0; i < n; ++i) {
    int32_t l, r, a;
    int64_t b;
    io >> l >> r >> a >> b;
    seg[i] = make_tuple(l, r, a, b);
    aux.push_back(compress(i << 2 | 2, l));
    aux.push_back(compress(i << 2, r));
  }

  for (int i = n; i < n + q; ++i) {
    bool op;
    io >> op;
    if (op == 0) {
      int32_t l, r, a;
      int64_t b;
      io >> l >> r >> a >> b;
      seg[i] = make_tuple(l, r, a, b);
      aux.push_back(compress(i << 2 | 2, l));
      aux.push_back(compress(i << 2, r));
    } else {
      int32_t p;
      io >> p;
      seg[i] = make_tuple(p, 0, 0, inf + 1);
      aux.push_back(compress(i << 2 | 1, p));
    }
  }

  radix_sort_u32<uint64_t>(aux.data(), aux.size(), [&](uint64_t x) {
    return static_cast<uint32_t>(x);
  });

  for (uint32_t lst = 0, cnt = 0; uint64_t pos : aux) {
    uint32_t high = static_cast<uint32_t>(pos >> 32);
    uint32_t low = static_cast<uint32_t>(pos);
    auto& x = seg[high >> 2];
    if (high & 1) {
      if (low != lst) {
        ++cnt;
        points.push_back(static_cast<int32_t>(low - offset));
        lst = low;
      }
      get<0>(x) = cnt - 1;
    } else {
      (high & 2 ? get<0>(x) : get<1>(x)) = cnt - (low == lst);
    }
  }

  LiChaoSegTreeOffline<Node> t(points.begin(), points.end());

  for (int i = 0; i < n; ++i) {
    auto [l, r, a, b] = seg[i];
    t.add_seg(l, r, make_pair(a, b));
  }

  for (int i = n; i < n + q; ++i) {
    auto [l, r, a, b] = seg[i];
    if (b > inf) {
      int64_t v = t.get(l);
      if (v >= inf) {
        io << "INFINITY\n";
      } else {
        io << v << '\n';
      }
    } else {
      t.add_seg(l, r, make_pair(a, b));
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