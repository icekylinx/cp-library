# Fixed-modulus determinant

Header: [`lib/math/linalg/det_fixed_mod.hpp`](../../../lib/math/linalg/det_fixed_mod.hpp).

Compute a square matrix determinant modulo compile-time P. With prime = true, use inverse-based elimination and AVX2; otherwise use Euclidean elimination.

## Types and requirements

`Matrix` must support mat[row][column]. Prefer vector<vector<uint64_t>> with entries reduced to [0, P). Require P > 0 and prime P when prime = true. The AVX2 branch uses signed 32-bit multiplication and unsigned 64-bit accumulation; require P < 2^31 and N * (P - 1)^2 + (P - 1) <= UINT64_MAX. The default N = 16 works for 998244353.

## Interface

| Interface | Behavior |
| --- | --- |
| `det_inplace<P, N = 16, prime = true>(mat, n)` | Compute in place, modifying the matrix. |
| `det<P, N = 16, prime = true>(mat, n)` | Copy into a 64-bit matrix before computing, preserving the input. |

## Complexity

The prime branch takes O(n^3 + n^2 log P) time and O(n) counter space; the composite branch is O(n^3 log P). The det wrapper adds O(n^2) time and space for copying.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/linalg/det_fixed_mod.hpp"

int main() {
  std::vector<std::vector<uint64_t>> a{{1, 2}, {3, 4}};
  auto saved = a;
  assert(det<998244353>(a, 2) == 998244351);
  assert(a == saved);
  assert(det_inplace<998244353>(a, 2) == 998244351);
  std::vector<std::vector<uint64_t>> b{{2, 1}, {1, 2}};
  assert((det<12, 16, false>(b, 2) == 3));
}
```

## Notes

The file enables GCC target("avx2") and requires a compatible x86 CPU. With prime = true, the in-place interface requires 64-bit entries because SIMD accesses use 64-bit slots; do not pass a uint32_t matrix directly. N must be positive.

## Related examples

- [graph/counting_spanning_trees_directed.cpp](../../../tests/library_checker/graph/counting_spanning_trees_directed.cpp)
- [graph/counting_spanning_trees_undirected.cpp](../../../tests/library_checker/graph/counting_spanning_trees_undirected.cpp)
- [linear_algebra/determinant_of_matrix.cpp](../../../tests/library_checker/linear_algebra/determinant_of_matrix.cpp)

## Further reading

- [Official checker evidence and timing limits](../../guides/verification.md)
