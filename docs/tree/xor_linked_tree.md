# XorLinkedTree

Header: [`lib/tree/xor_linked_tree.hpp`](../../lib/tree/xor_linked_tree.hpp).

Store a tree using degrees and XORs of neighbors, then process child-to-parent edges by leaf removal.

## Types and requirements

The input must be a connected tree without self-loops. With weighted = false, weights are omitted; otherwise T must support XOR operations. build destroys the stored degrees and neighbor information.

## Interface

| Interface | Behavior |
| --- | --- |
| `XorLinkedTree(n) / init(n)` | Create n vertices and remove all edges. |
| `add_edge(u, v)` | Add an unweighted edge. |
| `add_edge(u, v, w)` | Add an edge of weight w in the weighted version. |
| `build(root, callback)` | Call callback(child, parent), or callback(child, parent, weight) for weighted trees. The root also triggers one callback; explicitly skip child == root. |

## Complexity

Initialization takes O(n), each insertion O(1), and a complete build O(n). Space is O(n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <vector>
#include "lib/tree/xor_linked_tree.hpp"

int main() {
  XorLinkedTree<true, int> tree(3);
  tree.add_edge(0, 1, 4);
  tree.add_edge(1, 2, 7);
  std::vector<int> dist(3, 0);
  int edges = 0;
  tree.build(0, [&](uint32_t child, uint32_t parent, int weight) {
    if (child == 0) return;  // Skip the root callback.
    dist[parent] = std::max(dist[parent], dist[child] + weight);
    ++edges;
  });
  assert(edges == 2 && dist[0] == 11);
}
```

## Notes

For the root callback, parent is the internal sentinel 0 rather than an actual parent. Skip the root before accessing its parent or counting edges. This structure supports bottom-up tree DP rather than general neighbor iteration. Before processing again, call init and reinsert every edge.

## Related examples

- [tree/tree_diameter.cpp](../../tests/library_checker/tree/tree_diameter.cpp)
