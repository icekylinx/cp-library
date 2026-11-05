# Barrett64

Header: [`lib/math/modular/barrett64.hpp`](../../../lib/math/modular/barrett64.hpp).

Reduce an unsigned 64-bit integer modulo a fixed modulus using a precomputed reciprocal.

## Types and requirements

Set the modulus through the constructor or set_mod before use. This implementation uses the top bit to detect a negative remainder correction, so use the safe range 2 <= mod <= 2^63. The CHECK(mod >= 2) assertion alone does not imply support for every full-width modulus.

## Interface

| Interface | Behavior |
| --- | --- |
| `Barrett64(mod)` | Set the modulus and reciprocal. |
| `set_mod(mod)` | Change the modulus. |
| `reduce(uint64_t x)` | Return x % mod; x may span the full unsigned 64-bit range. |
| `Barrett` | An alias for Barrett64. |

## Complexity

Setting the modulus uses one integer division; reduce uses a fixed number of multiplications and shifts. Both take O(1) time and O(1) space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <limits>
#include "lib/math/modular/barrett64.hpp"

int main() {
  Barrett64 reduce(998244353);
  uint64_t x = std::numeric_limits<uint64_t>::max();
  assert(reduce.reduce(x) == x % 998244353);
  reduce.set_mod(17);
  assert(reduce.reduce(12345) == 3);
}
```

## Notes

The 64-bit version requires GNU __uint128_t for the intermediate product. The default constructor leaves mod and inv uninitialized; call set_mod before reduce.

## Related examples

- [enumerative_combinatorics/montmort_number.cpp](../../../tests/library_checker/enumerative_combinatorics/montmort_number.cpp)
