#pragma once

#include "lib/utils/debug.hpp"

template <uint32_t B = 24>
struct IntegerSet {
  static constexpr uint32_t L0 = B >= 6 ? B - 6 : 0;
  static constexpr uint32_t L1 = B >= 12 ? B - 12 : 0;
  static constexpr uint32_t L2 = B >= 18 ? B - 18 : 0;

  uint32_t n = 0;
  size_t counter = 0;
  std::vector<uint64_t> b0;
  std::vector<uint64_t> b1;
  std::vector<uint64_t> b2;
  uint64_t b3;

  IntegerSet() { build(1u << B); };
  explicit IntegerSet(uint32_t _n) { build(_n); }

  void build(uint32_t _n) {
    n = _n;
    counter = 0;
    CHECK(n <= (1 << B));
    b0.assign(1u << L0, 0);
    b1.assign(1u << L1, 0);
    b2.assign(1u << L2, 0);
    b3 = 0;
  }

  template <bool check = true>
  std::conditional_t<check, bool, void> insert(uint32_t pos) {
    CHECK(pos < n);
    if constexpr (check) {
      if (get(pos)) return false;
    }
    b0[pos >> 6] |= 1ull << (pos & 63);
    b1[pos >> 12] |= 1ull << (pos >> 6 & 63);
    b2[pos >> 18] |= 1ull << (pos >> 12 & 63);
    b3 |= 1ull << (pos >> 18);
    if constexpr (check) {
      ++counter;
      return true;
    }
  }

  template <bool check = true>
  std::conditional_t<check, bool, void> erase(uint32_t pos) {
    CHECK(pos < n);
    if constexpr (check) {
      if (!get(pos)) return false;
    }
    if (!(b0[pos >> 6] &= ~(1ull << (pos & 63))) &&
        !(b1[pos >> 12] &= ~(1ull << (pos >> 6 & 63))) &&
        !(b2[pos >> 18] &= ~(1ull << (pos >> 12 & 63)))) {
      b3 &= ~(1ull << (pos >> 18));
    }
    if constexpr (check) {
      --counter;
      return true;
    }
  }

  int prev(uint32_t pos) const {
    CHECK(pos < n);
    uint32_t u = pos >> 6, v = pos & 63, res;
    uint64_t mask = b0[u] & ((1ull << v) - 1);
    if (!mask) {
      v = u & 63, u >>= 6;
      mask = b1[u] & ((1ull << v) - 1);
      if (!mask) {
        v = u & 63, u >>= 6;
        mask = b2[u] & ((1ull << v) - 1);
        if (!mask) {
          mask = b3 & ((1ull << u) - 1);
          if (!mask) {
            return -1;
          } else {
            res = 63 ^ __builtin_clzll(mask);
            res = res << 6 | (63 ^ __builtin_clzll(b2[res]));
            res = res << 6 | (63 ^ __builtin_clzll(b1[res]));
            return res << 6 | (63 ^ __builtin_clzll(b0[res]));
          }
        } else {
          res = u << 6 | (63 ^ __builtin_clzll(mask));
          res = res << 6 | (63 ^ __builtin_clzll(b1[res]));
          return res << 6 | (63 ^ __builtin_clzll(b0[res]));
        }
      } else {
        res = u << 6 | (63 ^ __builtin_clzll(mask));
        return res << 6 | (63 ^ __builtin_clzll(b0[res]));
      }
    } else {
      return u << 6 | (63 ^ __builtin_clzll(mask));
    }
  }

  int next(uint32_t pos) const {
    CHECK(pos < n);
    uint32_t u = pos >> 6, v = pos & 63, res;
    uint64_t mask = b0[u] & ((-1ull << v) << 1);
    if (!mask) {
      v = u & 63, u >>= 6;
      mask = b1[u] & ((-1ull << v) << 1);
      if (!mask) {
        v = u & 63, u >>= 6;
        mask = b2[u] & ((-1ull << v) << 1);
        if (!mask) {
          mask = b3 & ((-1ull << u) << 1);
          if (!mask) {
            return -1;
          } else {
            res = __builtin_ctzll(mask);
            res = res << 6 | __builtin_ctzll(b2[res]);
            res = res << 6 | __builtin_ctzll(b1[res]);
            return res << 6 | __builtin_ctzll(b0[res]);
          }
        } else {
          res = u << 6 | __builtin_ctzll(mask);
          res = res << 6 | __builtin_ctzll(b1[res]);
          return res << 6 | __builtin_ctzll(b0[res]);
        }
      } else {
        res = u << 6 | __builtin_ctzll(mask);
        return res << 6 | __builtin_ctzll(b0[res]);
      }
    } else {
      return u << 6 | __builtin_ctzll(mask);
    }
  }

  bool get(uint32_t pos) const {
    CHECK(pos < n);
    return b0[pos >> 6] >> (pos & 63) & 1;
  }

  size_t size() const { return counter; }
};