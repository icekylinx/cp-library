#pragma once

#include "lib/utils/my_type_traits.hpp"

namespace internal {

template <basic_integral T>
constexpr int __lg(T x) {
  if constexpr (sizeof(T) <= 4) {
    return 31 ^ __builtin_clz(static_cast<uint32_t>(x));
  } else {
    return 63 ^ __builtin_clzll(static_cast<uint64_t>(x));
  }
}

template <basic_integral T>
constexpr int countl_zero(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_clz(static_cast<uint32_t>(x));
  } else {
    return __builtin_clzll(static_cast<uint64_t>(x));
  }
}

template <basic_integral T>
constexpr int countr_zero(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_ctz(static_cast<uint32_t>(x));
  } else {
    return __builtin_ctzll(static_cast<uint64_t>(x));
  }
}

template <basic_integral T>
constexpr int popcount(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_popcount(static_cast<uint32_t>(x));
  } else {
    return __builtin_popcountll(static_cast<uint64_t>(x));
  }
}

}  // namespace internal