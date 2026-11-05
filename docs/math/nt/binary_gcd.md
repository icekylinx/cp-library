# Binary GCD

Header: [`lib/math/nt/binary_gcd.hpp`](../../../lib/math/nt/binary_gcd.hpp).

Compute the greatest common divisor of two nonnegative integers using shifts and differences.

## Types and requirements

T must be a signed integer type, but argument values must be nonnegative. Negative arguments and the minimum signed value are outside the supported conditions. Differences and shifted results must also be representable.

## Interface

| Interface | Behavior |
| --- | --- |
| `binary_gcd(a, b)` | Return gcd(a, b); if one argument is zero, return the other, and if both are zero, return zero. |

## Complexity

O(log(max(a, b) + 1)) integer operations and O(1) space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/nt/binary_gcd.hpp"

int main() {
  assert(binary_gcd(48, 18) == 6);
  assert(binary_gcd(0LL, 27LL) == 27);
  assert(binary_gcd(0, 0) == 0);
}
```

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
