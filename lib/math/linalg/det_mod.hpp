#pragma once

#include "lib/utils/debug.hpp"
#include "lib/math/modular/barrett64.hpp"

template <typename Matrix>
uint32_t det_inplace(Matrix& mat, const uint32_t n, const uint32_t P) {
  CHECK(P > 0);
  if (P == 1) [[unlikely]] return 0;

  Barrett bar(P);
  bool neg = false;

  for (uint32_t i = 0; i < n; ++i) {
    for (uint32_t j = i; j < n; ++j) {
      if (mat[j][i] != 0) {
        if (j != i) {
          std::swap(mat[i], mat[j]);
          neg ^= 1;
        }
        break;
      }
    }
    for (uint32_t j = i + 1; j < n; ++j) {
      while (mat[i][i] != 0) {
        uint32_t inv = P - mat[j][i] / mat[i][i];
        if (inv != P) [[likely]] {
          for (uint32_t x = i; x < n; ++x) {
            mat[j][x] = bar.reduce(mat[j][x] + 1ull * inv * mat[i][x]);
          }
        }
        std::swap(mat[i], mat[j]);
        neg ^= 1;
      }
      std::swap(mat[i], mat[j]);
      neg ^= 1;
    }
  }

  uint32_t res = neg ? P - 1 : 1;
  for (uint32_t i = 0; i < n; ++i) {
    res = bar.reduce(1ull * res * mat[i][i]);
  }
  return res;
}

template <typename Matrix>
uint32_t det(Matrix mat, uint32_t n, uint32_t P) {
  return det_inplace(mat, n, P);
}