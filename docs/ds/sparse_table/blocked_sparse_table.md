# BlockedSparseTable

Header: [`lib/ds/sparse_table/blocked_sparse_table.hpp`](../../../lib/ds/sparse_table/blocked_sparse_table.hpp).

Maintain static interval aggregates for idempotent operations such as minimum, maximum, or gcd. Elements are grouped into blocks of B, with B = 16 by default.

## Types and requirements

`T` provides Info, id(), and op(a, b). The operation must be associative and idempotent, since query blocks may overlap; idempotence means op(x, x) = x. Intervals are [l, r), and an empty interval returns id().

## Interface

| Interface | Behavior |
| --- | --- |
| `BlockedSparseTable(first, last)` | Build from an iterator range. |
| `BlockedSparseTable(n, func)` | Generate initial values using func(i). |
| `build(first, last) / build(n, func)` | Rebuild the static table. |
| `prod(l, r)` | Return the interval aggregate; updates are not supported. |

## Complexity

Preprocessing and space are O(n + (n/B) log(n/B)). Queries within one block take O(B); queries crossing blocks take O(1). With fixed B = 16, queries have constant cost.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <limits>
#include <vector>
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"

struct Min {
  using Info = int;
  static Info id() { return std::numeric_limits<int>::max(); }
  static Info op(Info a, Info b) { return std::min(a, b); }
};
int main() {
  std::vector<int> a{5, 2, 8, 1};
  BlockedSparseTable<Min> table(a.begin(), a.end());
  assert(table.prod(0, 3) == 2);
  assert(table.prod(2, 4) == 1);
}
```

## Notes

Ordinary range sums are not supported because addition is not idempotent. B must be positive.

## Related examples

- [data_structure/static_rmq.cpp](../../../tests/library_checker/data_structure/static_rmq.cpp)
- [tree/lowest_common_ancestor.cpp](../../../tests/library_checker/tree/lowest_common_ancestor.cpp)

## Further reading

- [Idempotence](../../guides/algebra.md)
- [LCA recipe and its official constraints](../../recipes/lca.md)
