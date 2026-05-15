#pragma once

#include <cstdint>

#include "lib/ds/hash_map/hash_map_core.hpp"
#include "lib/math/hash/fast_hasher.hpp"

template <typename K, typename V, uint32_t N>
using HashMap = HashMapCore<K, V, N, FastHasher<K>>;
