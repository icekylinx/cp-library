# SparseTable

Header: [`lib/ds/sparse_table/sparse_table.hpp`](../../../lib/ds/sparse_table/sparse_table.hpp).

Maintain static interval aggregates for idempotent operations such as minimum, maximum, or gcd.

## Types and requirements

`T` provides Info, id(), and op(a, b). The operation must be associative and idempotent, since query blocks may overlap; idempotence means op(x, x) = x. Intervals are [l, r), and an empty interval returns id().

## Interface

| Interface | Behavior |
| --- | --- |
| `SparseTable(first, last)` | Build from an iterator range. |
| `SparseTable(n, func)` | Generate initial values using func(i). |
| `build(first, last) / build(n, func)` | Rebuild the static table. |
| `prod(l, r)` | Return the interval aggregate; updates are not supported. |

## Complexity

Preprocessing and space are O(n log n); each query takes O(1).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <limits>
#include <vector>
#include "lib/ds/sparse_table/sparse_table.hpp"

struct Min {
  using Info = int;
  static Info id() { return std::numeric_limits<int>::max(); }
  static Info op(Info a, Info b) { return std::min(a, b); }
};
int main() {
  std::vector<int> a{5, 2, 8, 1};
  SparseTable<Min> table(a.begin(), a.end());
  assert(table.prod(0, 3) == 2);
  assert(table.prod(2, 4) == 1);
}
```

## Notes

Ordinary range sums are not supported because addition is not idempotent.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
