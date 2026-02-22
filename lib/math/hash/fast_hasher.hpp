#pragma once

template <typename T>
struct FastHasher {
  static constexpr uint64_t Magic = 11400714819323198485ull;
  uint64_t operator()(T raw) const {
    return Magic * static_cast<uint64_t>(raw);
  }
};