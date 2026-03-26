#pragma once

template <std::random_access_iterator It>
auto z_algo(It s, const uint32_t n) {
  if (n == 0) [[unlikely]] return std::vector<uint32_t>{};
  std::vector<uint32_t> z(n);
  z[0] = n;
  for (uint32_t i = 1, l = 0, r = 0; i < n; ++i) {
    if (i <= r && z[i - l] < r - i + 1) {
      z[i] = z[i - l];
    } else {
      z[i] = std::max(r + 1, i);
      while (z[i] != n && s[z[i]] == s[z[i] - i]) {
        ++z[i];
      }
      if (z[i] - 1 > r) {
        l = i, r = z[i] - 1;
      }
      z[i] -= i;
    }
  }
  return z;
}

template <typename String>
auto z_algo(const String& s) {
  return z_algo(s.data(), s.size());
}