# SplitMix64

Header: [`lib/math/hash/splitmix64.hpp`](../../../lib/math/hash/splitmix64.hpp).

A seeded 64-bit integer mixing function suitable for hash tables.

## Types and requirements

The constructor obtains a seed from steady_clock. The same object returns the same hash for the same input. operator() does not advance state, so this is not an interface that produces a new random number on every call.

## Interface

| Interface | Behavior |
| --- | --- |
| `SplitMix64()` | Initialize seed. |
| `operator()(uint64_t value)` | Return a 64-bit hash after adding the seed and applying two multiplication-based mixing rounds. |
| `seed` | A public writable seed; set it before the first table insertion when deterministic reproduction is needed. |

## Complexity

Construction, each call, and extra space are O(1).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/hash/splitmix64.hpp"

int main() {
  SplitMix64 hash;
  hash.seed = 123;
  assert(hash(42) == hash(42));
  assert(hash(42) != hash(43));
}
```

## Notes

Do not change a table hasher's seed after insertion, or existing keys will no longer map to their original slots. A clock-based seed does not provide cryptographic security.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
