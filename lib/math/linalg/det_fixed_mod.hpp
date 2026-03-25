#pragma once

#pragma GCC target("avx2")
#include <immintrin.h>

#include "lib/utils/debug.hpp"

template <uint32_t P, uint32_t N = 16, bool prime = true, typename Matrix>
uint32_t det_inplace(Matrix& mat, const uint32_t n) {
  CHECK(P > 0);
  if constexpr (P == 1) return 0;

  bool neg = false;

  if constexpr (prime) {
    auto qpow = [&](uint32_t x, uint32_t y, uint32_t k = 1) -> uint32_t {
      for (; y; y >>= 1, x = 1ull * x * x % P) {
        if (y & 1) k = 1ull * k * x % P;
      }
      return k;
    };

    std::vector<uint32_t> counter(n);
    for (uint32_t i = 0; i < n; ++i) {
      for (uint32_t j = i; j < n; ++j) {
        mat[j][i] %= P;
      }
      for (uint32_t j = i; j < n; ++j) {
        if (mat[j][i] != 0) {
          if (j != i) {
            std::swap(mat[i], mat[j]);
            counter[j] = counter[i];
            neg ^= 1;
          }
          break;
        }
      }
      for (uint32_t j = i; j < n; ++j) {
        mat[i][j] %= P;
      }
      if (mat[i][i] == 0) [[unlikely]] return 0;
      uint32_t inv = P - qpow(mat[i][i], P - 2);
      for (uint32_t j = i + 1; j < n; ++j) {
        uint32_t factor = inv * mat[j][i] % P;
        const __restrict auto& a = mat[i];
        __restrict auto& b = mat[j];
        uint32_t x = i;
        __m256i vc = _mm256_set1_epi32(factor);
        for (; x + 4 <= n; x += 4) {
          __m256i va =
              _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&a[x]));
          __m256i vb =
              _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&b[x]));
          __m256i vmul = _mm256_mul_epi32(va, vc);
          vb = _mm256_add_epi64(vb, vmul);
          _mm256_storeu_si256(reinterpret_cast<__m256i*>(&b[x]), vb);
        }
        for (; x < n; ++x) {
          b[x] += factor * a[x];
        }
        if (++counter[j] == N) {
          counter[j] = 0;
          for (uint32_t x = i; x < n; ++x) {
            b[x] %= P;
          }
        }
      }
    }
  } else {
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
      if (mat[i][i] == 0) [[unlikely]] return 0;
      for (uint32_t j = i + 1; j < n; ++j) {
        while (mat[i][i] != 0) {
          uint32_t inv = P - mat[j][i] / mat[i][i];
          if (inv != P) [[likely]] {
            for (uint32_t x = i; x < n; ++x) {
              mat[j][x] = (mat[j][x] + 1ull * inv * mat[i][x]) % P;
            }
          }
          std::swap(mat[i], mat[j]);
          neg ^= 1;
        }
        std::swap(mat[i], mat[j]);
        neg ^= 1;
      }
    }
  }

  uint32_t res = neg ? P - 1 : 1;
  for (uint32_t i = 0; i < n; ++i) {
    res = res * mat[i][i] % P;
  }
  return res;
}

template <uint32_t P, uint32_t N = 16, bool prime = true, typename Matrix>
uint32_t det(const Matrix& mat, const uint32_t n) {
  std::vector<std::vector<uint64_t>> a(n, std::vector<uint64_t>(n));
  for (uint32_t i = 0; i < n; ++i) {
    for (uint32_t j = 0; j < n; ++j) {
      a[i][j] = mat[i][j];
    }
  }
  return det<P, N, prime>(a, n);
}