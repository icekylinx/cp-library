// https://judge.yosupo.jp/problem/line_add_get_min

#include <bits/stdc++.h>
#include "lib/utils/fast_io.hpp"
#include "lib/ds/li_chao_tree/li_chao_tree.hpp"
using namespace std;

FastIO<1 << 20, 1 << 20> io;

struct Node {
  using Line  = pair<int32_t, int64_t>;
  using Value = int64_t;
  using Range = int32_t;

  static constexpr Line id() { return make_pair(0, 1e18); }

  static constexpr pair<Range, Range> range_id() {
    return make_pair(-1e9 - 5, 1e9 + 5);
  }

  static constexpr Value evaluate(const Line& line, Range x) {
    return static_cast<Value>(line.first) * x + line.second;
  }

  static constexpr bool compare(Value x, Value y) { return x < y; }

  static constexpr Value op(Value x, Value y) { return min(x, y); }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  LiChaoTree<Node> t((n + q) * 10);

  while (n--) {
    int32_t a;
    int64_t b;
    io >> a >> b;
    t.add({a, b});
  }

  while (q--) {
    bool op;
    io >> op;
    if (op == 0) {
      int32_t a;
      int64_t b;
      io >> a >> b;
      t.add(make_pair(a, b));
    } else {
      int32_t x;
      io >> x;
      io << t.get(x) << '\n';
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