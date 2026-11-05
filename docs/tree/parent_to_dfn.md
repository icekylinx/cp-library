# Parent array to DFS order

Header: [`lib/tree/parent_to_dfn.hpp`](../../lib/tree/parent_to_dfn.hpp).

Convert a parent array satisfying an ordering constraint into DFS preorder positions in linear time.

## Types and requirements

Vertices are numbered [0, n), vertex 0 is the root, and fa[i] < i for every i >= 1. fa[0] is ignored. The returned dfn[i] is the preorder position of vertex i; the root has position zero.

## Interface

| Interface | Behavior |
| --- | --- |
| `parent_to_dfn(first, n)` | Read n parent indices and return the preorder-position array. |
| `parent_to_dfn(parent)` | Container overload requiring data() and size(). |

## Complexity

O(n) time and O(n) extra space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <vector>
#include "lib/tree/parent_to_dfn.hpp"

int main() {
  std::vector<uint32_t> parent{0, 0, 0, 1};
  auto dfn = parent_to_dfn(parent);
  assert(dfn[0] == 0);
  assert(dfn[1] + 1 == dfn[3]);
  std::sort(dfn.begin(), dfn.end());
  assert((dfn == std::vector<uint32_t>{0, 1, 2, 3}));
}
```

## Notes

Sibling-subtree order is implementation-defined; do not assume adjacency-list order. The result maps vertices to positions rather than listing vertices in visit order. Subtree sizes and interval endpoints are not returned.

## Related examples

- [tree/lowest_common_ancestor.cpp](../../tests/library_checker/tree/lowest_common_ancestor.cpp)
