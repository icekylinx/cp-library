# FastHasher

Header: [`lib/math/hash/fast_hasher.hpp`](../../../lib/math/hash/fast_hasher.hpp).

Multiply an integer key by a fixed odd constant to produce an unsigned 64-bit hash.

## Types and requirements

`T` must be explicitly convertible to uint64_t. The hash is deterministic and is intended primarily for fixed-capacity tables that use the high bits.

## Interface

| Interface | Behavior |
| --- | --- |
| `FastHasher<T>{}(value)` | Return Magic * uint64_t(value), with ordinary uint64_t wraparound. |
| `FastHasher<T>::Magic` | The fixed multiplier 11400714819323198485. |

## Complexity

O(1) time and extra space per call.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/hash/fast_hasher.hpp"

int main() {
  FastHasher<uint64_t> hash;
  assert(hash(0) == 0);
  assert(hash(1) == FastHasher<uint64_t>::Magic);
}
```

## Notes

This hash provides neither randomization nor cryptographic collision resistance.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
