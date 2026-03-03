#pragma once

#include "lib/utils/my_type_traits.hpp"
#include "lib/math/my_bit.hpp"

namespace internal {

// Based on https://en.algorithmica.org/hpc/algorithms/gcd/
template <basic_signed_integral T>
T binary_gcd(T a, T b) {
  CHECK(a >= 0 && b >= 0);
  if (a == 0 || b == 0) return a + b;

  int az = countr_zero(a);
  int bz = countr_zero(b);
  int shift = std::min(az, bz);
  b >>= bz;

  while (a != 0) {
    a >>= az;
    T diff = b - a;
    if (diff == 0) break;
    az = countr_zero(diff);
    b = std::min(a, b);
    a = std::abs(diff);
  }

  return b << shift;
}

}  // namespace internal