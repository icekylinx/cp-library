#pragma once

#include <cstdint>
#include <iterator>

// Based on https://en.algorithmica.org/hpc/data-structures/binary-search/
template <typename T, std::random_access_iterator It, typename F>
It fast_lower_bound(It f, uint32_t n, const T& x, F&& comp) {
  for (++n; n > 1; ) {
    uint32_t half = n / 2;
    n -= half;
    f += comp(f[half - 1], x) * half;
  }
  return f;
}

template <typename T, std::random_access_iterator It, typename F>
It fast_upper_bound(It f, uint32_t n, const T& x, F&& comp) {
  for (++n; n > 1; ) {
    uint32_t half = n / 2;
    n -= half;
    f += (!comp(x, f[half - 1])) * half;
  }
  return f;
}
