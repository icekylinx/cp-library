// https://judge.yosupo.jp/problem/line_add_get_min

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/li_chao_tree/li_chao_tree_offline.hpp"
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
  vector<pair<int, int64_t>> line;
  line.resize(n + q);
  vector<uint64_t> aux;
  aux.reserve(q);

  uint32_t offset = 1u << 31;

  auto compress = [&](int i, int32_t v) -> uint64_t {
    uint32_t tmp = static_cast<uint32_t>(v) + offset;
    return static_cast<uint64_t>(i) << 32 | tmp;
  };

  for (int i = 0; i < n; ++i) {
    int32_t a;
    int64_t b;
    io >> a >> b;
    line[i] = make_pair(a, b);
  }

  for (int i = n; i < n + q; ++i) {
    bool op;
    io >> op;
    if (op == 0) {
      int32_t a;
      int64_t b;
      io >> a >> b;
      line[i] = make_pair(a, b);
    } else {
      int32_t p;
      io >> p;
      line[i] = make_pair(p, inf + 1);
      aux.push_back(compress(i, p));
    }
  }

  radix_sort_u32<uint64_t>(aux.data(), aux.size(), [&](uint64_t x) {
    return static_cast<uint32_t>(x);
  });

  vector<int> points(aux.size());

  for (uint32_t i = 0; i < aux.size(); ++i) {
    uint64_t pos = aux[i];
    uint32_t high = static_cast<uint32_t>(pos >> 32);
    uint32_t low = static_cast<uint32_t>(pos);
    points[i] = static_cast<int32_t>(low - offset);
    line[high].first = i;
  }

  LiChaoTreeOffline<Node> t(points.begin(), points.end());

  for (int i = 0; i < n; ++i) {
    t.add(line[i]);
  }

  for (int i = n; i < n + q; ++i) {
    auto [l, r] = line[i];
    if (r > inf) {
      io << t.get(l) << '\n';
    } else {
      t.add(line[i]);
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