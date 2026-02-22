// https://judge.yosupo.jp/problem/predecessor_problem

#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
#include "lib/utils/my_simd.hpp"
#include "lib/utils/fast_io.hpp"
#include "lib/ds/set/integer_set.hpp"
using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

IntegerSet<24> s;

void solve_main() {
  int n, q;
  io >> n >> q;

  auto& ptr = io.in->cur;

  auto ones = _mm256_set1_epi8('1');
  for (int i = 0, _i = n - 63; i < _i; i += 64) {
    auto v0 = _mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(ptr));
    auto v1 = _mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(ptr + 32));
    auto cmp0 = _mm256_cmpeq_epi8(v0, ones);
    auto cmp1 = _mm256_cmpeq_epi8(v1, ones);
    auto mask0 = static_cast<uint32_t>(_mm256_movemask_epi8(cmp0));
    auto mask1 = static_cast<uint32_t>(_mm256_movemask_epi8(cmp1));
    s.b0[i >> 6] = static_cast<uint64_t>(mask1) << 32 | mask0;
    ptr += 64;
  }

  if (true) {
    uint64_t mask = 0;
    int i = n / 64 * 64;
    for (int j = 0; j < n - i; ++j) {
      mask |= static_cast<uint64_t>(*ptr++ & 1) << j;
    }
    s.b0[i >> 6] = mask;
  }

  for (int i = 0, _i = n >> 6; i < _i; i += 64) {
    uint64_t mask = 0;
    for (int j = 0; j < 64; ++j) {
      mask |= static_cast<uint64_t>(!!s.b0[i + j]) << j;
    }
    s.b1[i >> 6] = mask;
  }

  for (int i = 0, _i = n >> 12; i < _i; i += 64) {
    uint64_t mask = 0;
    for (int j = 0; j < 64; ++j) {
      mask |= static_cast<uint64_t>(!!s.b1[i + j]) << j;
    }
    s.b2[i >> 6] = mask;
  }

  s.b3 = 0;
  for (int j = 0; j < 64; ++j) {
    s.b3 |= static_cast<uint64_t>(!!s.b2[j]) << j;
  }

  while (q--) {
    char op;
    uint32_t k;
    io.in->skip_space();
    io.in->read(op);
    io.in->read<uint32_t, 0>(k);
    if (op == '0') {
      s.insert<false>(k);
    } else if (op == '1') {
      s.erase<false>(k);
    } else if (op == '2') {
      io << s.get(k) << '\n';
    } else if (op == '3') {
      if (k != 0) {
        io << s.next(k - 1) << '\n';
      } else {
        io << (s.get(0) ? 0 : s.next(0)) << '\n';
      }
    } else {
      io << s.prev(k + 1) << '\n';
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