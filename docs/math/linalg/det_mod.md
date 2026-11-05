# Runtime-modulus determinant

Header: [`lib/math/linalg/det_mod.hpp`](../../../lib/math/linalg/det_mod.hpp).

Compute a determinant modulo a runtime modulus using Euclidean elimination, including for composite moduli.

## Types and requirements

Require P > 0 and entries reduced to [0, P). Prefer uint64_t entries to hold intermediate multiply-add results. Matrix must support row swapping and double subscripting.

## Interface

| Interface | Behavior |
| --- | --- |
| `det_inplace(mat, n, P)` | Eliminate in place and return the determinant modulo P. |
| `det(mat, n, P)` | Copy the matrix by value before computing, preserving the caller's input. |

## Complexity

Euclidean elimination takes O(n^3 log P) time. The in-place version needs O(1) extra space beyond the matrix; the copying version adds O(n^2) space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/linalg/det_mod.hpp"

int main() {
  std::vector<std::vector<uint64_t>> a{{2, 1}, {1, 2}};
  auto saved = a;
  assert(det(a, 2, 12) == 3);
  assert(a == saved);
  assert(det_inplace(a, 2, 12) == 3);
  assert(det(a, 2, 1) == 0);
}
```

## Notes

Modulo 1, the result is zero. For large moduli, ensure multiply-add expressions are evaluated in unsigned 64-bit arithmetic. This interface does not require declaring the modulus prime.

## Related examples

- [linear_algebra/determinant_of_matrix_arbitrary_mod.cpp](../../../tests/library_checker/linear_algebra/determinant_of_matrix_arbitrary_mod.cpp)

## Further reading

- [Official checker evidence and timing limits](../../guides/verification.md)
