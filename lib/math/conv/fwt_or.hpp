#pragma once

#include "lib/utils/debug.hpp"

template <uint32_t P, uint32_t N, std::random_access_iterator It>
void fwt_or(It f) {
  auto add = [&](uint32_t x, uint32_t y) -> uint32_t {
    x += y;
    return std::min(x, x - P);
  };

  static constexpr uint32_t len = N >> 1;

  if constexpr (len > 0) {
    fwt_or<P, len>(f);
    fwt_or<P, len>(f + len);
    for (uint32_t i = 0; i < len; ++i) {
      f[i + len] = add(f[i], f[i + len]);
    }
  }
}

template <uint32_t P, uint32_t N, std::random_access_iterator It>
void ifwt_or(It f) {
  auto sub = [&](uint32_t x, uint32_t y) -> uint32_t {
    x -= y;
    return std::min(x, x + P);
  };

  static constexpr uint32_t len = N >> 1;

  if constexpr (len > 0) {
    ifwt_or<P, len>(f);
    ifwt_or<P, len>(f + len);
    for (uint32_t i = 0; i < len; ++i) {
      f[i + len] = sub(f[i + len], f[i]);
    }
  }
}

template <uint32_t P, std::random_access_iterator It, uint32_t N = 0>
void fwt_or(It f, const uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      fwt_or<P, 1u << N>(f);
    } else {
      fwt_or<P, It, N + 1>(f, n);
    }
  }
}

template <uint32_t P, std::random_access_iterator It, uint32_t N = 0>
void ifwt_or(It f, const uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      ifwt_or<P, 1u << N>(f);
    } else {
      ifwt_or<P, It, N + 1>(f, n);
    }
  }
}

template <uint32_t P, typename Poly>
void fwt_or(Poly& f) {
  fwt_or<P>(f.data(), f.size());
}

template <uint32_t P, typename Poly>
void ifwt_or(Poly& f) {
  ifwt_or<P>(f.data(), f.size());
}