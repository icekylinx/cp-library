// https://judge.yosupo.jp/problem/aplusb

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>

using ll  = long long;
using ull = unsigned long long;

void solve_main() {
  uint32_t a, b;
  std::cin >> a >> b;
  std::cout << a + b << '\n';
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  std::cin.tie(nullptr)->sync_with_stdio(false);
  solve_main();
  return 0;
}
