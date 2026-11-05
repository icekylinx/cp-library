# LiChaoTree

Header: [`lib/ds/li_chao_tree/li_chao_tree.hpp`](../../../lib/ds/li_chao_tree/li_chao_tree.hpp).

Dynamically insert whole lines and query the best value over an integer coordinate domain.

## Types and requirements

`T` provides Line, Value, Range, id(), range_id(), evaluate(line, x), compare(a, b), and op(a, b). For minimum queries, compare is less-than, op is min, and id is a sentinel line that never beats a real line anywhere in the coordinate domain. Pairs of lines must satisfy the single-crossing property used by Li Chao trees. Evaluation and coordinate arithmetic must not overflow. Here range_id() returns pair{L, R} for an integer half-open domain with L < R, and query points must satisfy L <= x < R.

## Interface

| Interface | Behavior |
| --- | --- |
| `LiChaoTree() / LiChaoTree(reserved)` | Create an empty tree; the argument reserves node capacity, not coordinate-domain length. |
| `build(reserved = 0)` | Remove all lines and rebuild the root. |
| `reserve(size)` | Reserve node-array capacity. |
| `add(line)` | Insert a line over the entire coordinate domain. |
| `get(x)` | Query the best value at the actual coordinate x. |

## Complexity

For domain width U = R - L, insertion and queries take O(log U). Node space after q insertions is at most O(q log U).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <utility>
#include "lib/ds/li_chao_tree/li_chao_tree.hpp"

struct Lines {
  using Line = std::pair<long long, long long>;
  using Value = long long;
  using Range = int;
  static Line id() { return {0, 1LL << 60}; }
  static Value evaluate(Line line, Range x) { return line.first * x + line.second; }
  static bool compare(Value a, Value b) { return a < b; }
  static Value op(Value a, Value b) { return std::min(a, b); }
  static std::pair<int, int> range_id() { return {-10, 11}; }
};
int main() {
  LiChaoTree<Lines> tree;
  tree.add({2, 1});
  tree.add({-1, 4});
  assert(tree.get(0) == 1);
  assert(tree.get(3) == 1);
}
```

## Notes

get accepts an actual coordinate; offline variants accept a compressed index instead. new_node is a low-level interface and is not needed for normal use.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.

## Further reading

- [Coordinate domains and offline variants](../../guides/offline_coordinates.md)
