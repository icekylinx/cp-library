#pragma once

#include "lib/utils/debug.hpp"

template <typename T, uint32_t N = 16, std::random_access_iterator It,
          typename F>
void radix_sort_u32(It f, const uint32_t n, F&& func) {
  CHECK(N <= 16);

  static constexpr uint32_t Mask = (1u << N) - 1;

  if (n < Mask / 8) {
    std::stable_sort(f, f + n, [&](const T& x, const T& y) {
      return func(x) < func(y);
    });
    return;
  }

  std::array<uint32_t, Mask + 1> cnt1{}, cnt2{};
  for (uint32_t i = 0; i < n; ++i) {
    uint32_t val = func(f[i]);
    ++cnt1[val & Mask];
    ++cnt2[val >> N];
  }

  for (uint32_t i = 0; i < Mask; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
  }

  std::vector<T> tmp(n);
  for (uint32_t i = n; i--; ) {
    tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
  }
  for (uint32_t i = n; i--; ) {
    f[--cnt2[func(tmp[i]) >> N]] = std::move(tmp[i]);
  }
}

template <typename T, uint32_t N = 16, std::random_access_iterator It,
          typename F>
void radix_sort_u64(It f, const uint32_t n, F&& func) {
  CHECK(N <= 16);

  static constexpr uint32_t Mask = (1u << N) - 1;
  static constexpr uint32_t N2 = N * 2;
  static constexpr uint32_t N3 = N * 3;

  if (n < Mask / 8) {
    std::stable_sort(f, f + n, [&](const T& x, const T& y) {
      return func(x) < func(y);
    });
    return;
  }

  std::array<uint32_t, Mask + 1> cnt1{}, cnt2{}, cnt3{}, cnt4{};
  for (uint32_t i = 0; i < n; ++i) {
    uint64_t val = func(f[i]);
    ++cnt1[val & Mask];
    ++cnt2[val >> N & Mask];
    ++cnt3[val >> N2 & Mask];
    ++cnt4[val >> N3];
  }

  for (uint32_t i = 0; i < Mask; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
    cnt3[i + 1] += cnt3[i];
    cnt4[i + 1] += cnt4[i];
  }

  std::vector<T> tmp(n);
  for (uint32_t i = n; i--; ) {
    tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
  }
  for (uint32_t i = n; i--; ) {
    f[--cnt2[func(tmp[i]) >> N & Mask]] = std::move(tmp[i]);
  }
  for (uint32_t i = n; i--; ) {
    tmp[--cnt3[func(f[i]) >> N2 & Mask]] = std::move(f[i]);
  }
  for (uint32_t i = n; i--; ) {
    f[--cnt4[func(tmp[i]) >> N3]] = std::move(tmp[i]);
  }
}

template <typename T, uint32_t N = 16, typename Array, typename F>
void radix_sort_u32(Array& a, F&& func) {
  radix_sort_u32<T, N>(a.data(), a.size(), func);
}

template <typename T, uint32_t N = 16, typename Array, typename F>
void radix_sort_u64(Array& a, F&& func) {
  radix_sort_u64<T, N>(a.data(), a.size(), func);
}