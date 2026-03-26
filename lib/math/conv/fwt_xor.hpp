#pragma once

#include "lib/utils/debug.hpp"

template <uint32_t P, uint32_t N, std::random_access_iterator It>
void fwt_xor(It f) {
  auto add = [&](uint32_t x, uint32_t y) -> uint32_t {
    x += y;
    return std::min(x, x - P);
  };

  auto sub = [&](uint32_t x, uint32_t y) -> uint32_t {
    x -= y;
    return std::min(x, x + P);
  };

  static constexpr uint32_t len = N >> 1;

  if constexpr (len > 0) {
    fwt_xor<P, len>(f);
    fwt_xor<P, len>(f + len);
    for (uint32_t i = 0; i < len; ++i) {
      uint32_t x = add(f[i], f[i + len]);
      uint32_t y = sub(f[i], f[i + len]);
      f[i] = x, f[i + len] = y;
    }
  }
}

template <uint32_t P, uint32_t N, std::random_access_iterator It>
void ifwt_xor(It f) {
  auto add = [&](uint32_t x, uint32_t y) -> uint32_t {
    x += y;
    return std::min(x, x - P);
  };

  auto sub = [&](uint32_t x, uint32_t y) -> uint32_t {
    x -= y;
    return std::min(x, x + P);
  };

  auto reduce = [&](uint32_t x) -> uint32_t {
    return std::min(x, x - P);
  };

  static constexpr uint32_t len = N >> 1;

  if constexpr (len > 0) {
    ifwt_xor<P, len>(f);
    ifwt_xor<P, len>(f + len);
    for (uint32_t i = 0; i < len; ++i) {
      uint32_t x = add(f[i], f[i + len]);
      uint32_t y = sub(f[i], f[i + len]);
      f[i] = reduce((x + (x & 1) * P) >> 1);
      f[i + len] = reduce((y + (y & 1) * P) >> 1);
    }
  }
}

template <uint32_t P, std::random_access_iterator It, uint32_t N = 0>
void fwt_xor(It f, const uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      fwt_xor<P, 1u << N>(f);
    } else {
      fwt_xor<P, It, N + 1>(f, n);
    }
  }
}

template <uint32_t P, std::random_access_iterator It, uint32_t N = 0>
void ifwt_xor(It f, const uint32_t n) {
  if constexpr (N <= 30) {
    if (n <= 1u << N) {
      ifwt_xor<P, 1u << N>(f);
    } else {
      ifwt_xor<P, It, N + 1>(f, n);
    }
  }
}

template <uint32_t P, typename Poly>
void fwt_xor(Poly& f) {
  fwt_xor<P>(f.data(), f.size());
}

template <uint32_t P, typename Poly>
void ifwt_xor(Poly& f) {
  ifwt_xor<P>(f.data(), f.size());
}