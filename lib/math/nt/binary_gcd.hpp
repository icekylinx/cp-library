#pragma once

// Based on https://en.algorithmica.org/hpc/algorithms/gcd/
template <std::signed_integral T>
T binary_gcd(T a, T b) {
  using U = std::make_unsigned_t<T>;
  if (a == 0 || b == 0) return a + b;

  int az = std::countr_zero(static_cast<U>(a));
  int bz = std::countr_zero(static_cast<U>(b));
  int shift = std::min(az, bz);
  b >>= bz;

  while (a != 0) {
    a >>= az;
    T diff = b - a;
    if (diff == 0) break;
    az = std::countr_zero(static_cast<U>(diff));
    b = std::min(a, b);
    a = std::abs(diff);
  }

  return b << shift;
}