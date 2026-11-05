# DSU

Header: [`lib/ds/dsu/dsu.hpp`](../../../lib/ds/dsu/dsu.hpp).

Maintain disjoint sets and connectivity using path compression and union by size.

## Types and requirements

Vertices are numbered [0, n). After default construction, call resize before use; resize discards all previous sets.

## Interface

| Interface | Behavior |
| --- | --- |
| `DSU(n) / resize(n)` | Create n singleton sets. |
| `find(x)` | Return the representative and compress the search path. |
| `merge(x, y)` | Merge the sets; return true if they were distinct and false if already connected. |
| `same(x, y)` | Check whether two vertices belong to the same set. |
| `size(x)` | Return the size of the set containing x. |

## Complexity

Initialization and space are O(n). The amortized cost of find, merge, same, and size is O(alpha(n)).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/dsu/dsu.hpp"

int main() {
  DSU dsu(4);
  assert(dsu.merge(0, 1));
  assert(!dsu.merge(1, 0));
  assert(dsu.same(0, 1));
  assert(dsu.size(1) == 2);
}
```

## Notes

Representatives can change after merge. Splitting and rollback are not supported.

## Related examples

- [data_structure/unionfind.cpp](../../../tests/library_checker/data_structure/unionfind.cpp)
- [graph/minimum_spanning_tree.cpp](../../../tests/library_checker/graph/minimum_spanning_tree.cpp)
