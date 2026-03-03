#pragma once

#include "lib/utils/debug.hpp"

template <uint32_t P, int N, typename It>
void fwt_and(It f) {
  auto add = [&](uint32_t x, uint32_t y) -> uint32_t {
    x += y;
    return std::min(x, x - P);
  };

  static constexpr int len = N >> 1;

  if constexpr (len > 0) {
    fwt_and<P, len>(f);
    fwt_and<P, len>(f + len);
    for (int i = 0; i < len; ++i) {
      f[i] = add(f[i], f[i + len]);
    }
  }
}

template <uint32_t P, int N, typename It>
void ifwt_and(It f) {
  auto sub = [&](uint32_t x, uint32_t y) -> uint32_t {
    x -= y;
    return std::min(x, x + P);
  };

  static constexpr int len = N >> 1;

  if constexpr (len > 0) {
    ifwt_and<P, len>(f);
    ifwt_and<P, len>(f + len);
    for (int i = 0; i < len; ++i) {
      f[i] = sub(f[i], f[i + len]);
    }
  }
}

template <uint32_t P, typename It, uint32_t N = 0>
void fwt_and(It f, uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      fwt_and<P, 1u << N>(f);
    } else {
      fwt_and<P, It, N + 1>(f, n);
    }
  }
}

template <uint32_t P, typename It, uint32_t N = 0>
void ifwt_and(It f, uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      ifwt_and<P, 1u << N>(f);
    } else {
      ifwt_and<P, It, N + 1>(f, n);
    }
  }
}