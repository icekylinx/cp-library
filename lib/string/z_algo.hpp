#pragma once

template <std::random_access_iterator It>
std::vector<int> z_algo(It s, uint32_t n) {
  std::vector<int> z(n);
  z[0] = n;
  for (int i = 1, l = 0, r = 0; i < static_cast<int>(n); ++i) {
    if (i <= r && z[i - l] < r - i + 1) {
      z[i] = z[i - l];
    } else {
      z[i] = std::max(0, r - i + 1);
      while (i + z[i] != static_cast<int>(n) && s[z[i]] == s[i + z[i]]) {
        ++z[i];
      }
      if (i + z[i] - 1 > r) {
        l = i, r = i + z[i] - 1;
      }
    }
  }
  return z;
}