#pragma once

#include "lib/utils/debug.hpp"

struct Barrett32 {
  uint32_t mod, inv;

  Barrett32() = default;
  explicit constexpr Barrett32(uint32_t m) {
    set_mod(m);
  }

  constexpr void set_mod(uint32_t m) {
    CHECK(m >= 2);
    mod = m;
    inv = -1u / m + 1;
  }

  constexpr uint32_t reduce(uint32_t x) const {
    x -= static_cast<uint32_t>(1ull * x * inv >> 32) * mod;
    x += -(x >> 31) & mod;
    return x;
  }
};