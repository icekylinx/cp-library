# Barrett32

Header: [`lib/math/modular/barrett32.hpp`](../../../lib/math/modular/barrett32.hpp).

Reduce an unsigned 32-bit integer modulo a fixed modulus using a precomputed reciprocal.

## Types and requirements

Set the modulus through the constructor or set_mod before use. This implementation uses the top bit to detect a negative remainder correction, so use the safe range 2 <= mod <= 2^31. The CHECK(mod >= 2) assertion alone does not imply support for every full-width modulus.

## Interface

| Interface | Behavior |
| --- | --- |
| `Barrett32(mod)` | Set the modulus and reciprocal. |
| `set_mod(mod)` | Change the modulus. |
| `reduce(uint32_t x)` | Return x % mod; x may span the full unsigned 32-bit range. |

## Complexity

Setting the modulus uses one integer division; reduce uses a fixed number of multiplications and shifts. Both take O(1) time and O(1) space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <limits>
#include "lib/math/modular/barrett32.hpp"

int main() {
  Barrett32 reduce(998244353);
  uint32_t x = std::numeric_limits<uint32_t>::max();
  assert(reduce.reduce(x) == x % 998244353);
  reduce.set_mod(17);
  assert(reduce.reduce(12345) == 3);
}
```

## Notes

This version accepts only 32-bit inputs. To reduce a product of two 32-bit integers, use the 64-bit reducer or ordinary uint64_t remainder. The default constructor leaves mod and inv uninitialized; call set_mod before reduce.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
