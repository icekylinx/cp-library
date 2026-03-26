#pragma once

#include "lib/utils/debug.hpp"

template <std::random_access_iterator It>
auto parent_to_dfn(It fa, const uint32_t n) {
  if (n == 0) [[unlikely]] return std::vector<uint32_t>{};
  std::vector<uint32_t> dfn(n);
  for (uint32_t i = n - 1; i >= 1; --i) {
    CHECK(static_cast<uint32_t>(fa[i]) < i);
    dfn[fa[i]] += dfn[i] + 1;
  }
  for (uint32_t i = 1; i < n; ++i) {
    dfn[i] = std::exchange(dfn[fa[i]], dfn[fa[i]] - dfn[i] - 1);
  }
  return dfn;
}

template <typename Parent>
auto parent_to_dfn(const Parent& fa) {
  return parent_to_dfn(fa.data(), fa.size());
}