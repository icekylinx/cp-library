# FastHashMap

Header: [`lib/ds/hash_map/fast_hash_map.hpp`](../../../lib/ds/hash_map/fast_hash_map.hpp).

A hash map with exactly 2^N slots. Multiplicative hashing with fixed-capacity lookup and clearing.

## Types and requirements

`0 < N < 32`. Keys must support equality comparison, and values must be default-constructible. The table never grows; keep at least one slot empty, or an absent-key lookup or insertion may not terminate. Keys must be convertible to uint64_t.

## Interface

| Interface | Behavior |
| --- | --- |
| `operator[](key)` | Look up or insert a key; initialize new values with V{} and return a writable reference. |
| `get(key)` | Return the value by copy, or V{} if absent, without inserting. |
| `count(key)` | Check whether the key exists. |
| `clear()` | Clear occupancy markers; values are reinitialized when inserted again. |

## Complexity

Lookup and insertion are expected O(1) with a suitable hash distribution and controlled load, and O(2^N) in the worst case. Space is O(2^N). clear resets the whole bitmap.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/hash_map/fast_hash_map.hpp"

int main() {
  static FastHashMap<uint64_t, int, 10> map;
  map[42] = 7;
  assert(map.get(42) == 7);
  assert(map.get(99) == 0);
  assert(!map.count(99));
  map.clear();
  assert(!map.count(42));
}
```

## Notes

The object contains inline arrays; use static storage or the heap for large N. Individual deletion and iteration are not supported.

## Related examples

- [data_structure/associative_array.cpp](../../../tests/library_checker/data_structure/associative_array.cpp)
- [data_structure/static_range_count_distinct.cpp](../../../tests/library_checker/data_structure/static_range_count_distinct.cpp)
- [data_structure/static_range_frequency.cpp](../../../tests/library_checker/data_structure/static_range_frequency.cpp)
