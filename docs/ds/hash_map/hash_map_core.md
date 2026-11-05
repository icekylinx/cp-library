# HashMapCore

Header: [`lib/ds/hash_map/hash_map_core.hpp`](../../../lib/ds/hash_map/hash_map_core.hpp).

A hash map with exactly 2^N slots. A fixed-capacity linear-probing table with a configurable Hasher.

## Types and requirements

`0 < N < 32`. Keys must support equality comparison, and values must be default-constructible. The table never grows; keep at least one slot empty, or an absent-key lookup or insertion may not terminate. Hasher(key) must return a well-distributed unsigned 64-bit hash.

## Interface

| Interface | Behavior |
| --- | --- |
| `operator[](key)` | Look up or insert a key; initialize new values with V{} and return a writable reference. |
| `get(key)` | Return the value by copy, or V{} if absent, without inserting. |
| `count(key)` | Check whether the key exists. |
| `clear()` | Clear occupancy markers; values are reinitialized when inserted again. |
| `size()` | Return the number of distinct keys. |

## Complexity

Lookup and insertion are expected O(1) with a suitable hash distribution and controlled load, and O(2^N) in the worst case. Space is O(2^N). clear resets only recorded slots while the number of inserted keys is at most 1/128 of capacity; otherwise it resets the whole bitmap.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/hash_map/hash_map_core.hpp"
#include "lib/math/hash/fast_hasher.hpp"

int main() {
  static HashMapCore<uint64_t, int, 10, FastHasher<uint64_t>> map;
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

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
