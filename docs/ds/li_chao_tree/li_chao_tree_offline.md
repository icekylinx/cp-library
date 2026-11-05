# LiChaoTreeOffline

Header: [`lib/ds/li_chao_tree/li_chao_tree_offline.hpp`](../../../lib/ds/li_chao_tree/li_chao_tree_offline.hpp).

A Li Chao tree with query coordinates collected in advance. Supports insertion of whole lines.

## Types and requirements

`T` provides Line, Value, Range, id(), range_id(), evaluate(line, x), compare(a, b), and op(a, b). For minimum queries, compare is less-than, op is min, and id is a sentinel line that never beats a real line anywhere in the coordinate domain. Pairs of lines must satisfy the single-crossing property used by Li Chao trees. Evaluation and coordinate arithmetic must not overflow. Here range_id() returns a single sentinel coordinate at least as large as every query coordinate. Evaluating lines at the sentinel must also be safe. Supply at least one query point.

## Interface

| Interface | Behavior |
| --- | --- |
| `LiChaoTreeOffline(first, last) / build(first, last)` | Supply every query coordinate in nondecreasing order; duplicates are retained. |
| `get(i)` | Query the best value at coordinate number i; the argument is an index, not an actual coordinate. |
| `add(line)` | Insert a line at every query coordinate. |

## Complexity

For m query coordinates, initialization and space are O(m), and queries take O(log m). Whole-line insertion takes O(log m).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <utility>
#include <vector>
#include "lib/ds/li_chao_tree/li_chao_tree_offline.hpp"

struct Lines {
  using Line = std::pair<long long, long long>;
  using Value = long long;
  using Range = int;
  static Line id() { return {0, 1LL << 60}; }
  static Value evaluate(Line line, Range x) { return line.first * x + line.second; }
  static bool compare(Value a, Value b) { return a < b; }
  static Value op(Value a, Value b) { return std::min(a, b); }
  static int range_id() { return 100; }
};
int main() {
  std::vector<int> xs{-2, 0, 3};
  LiChaoTreeOffline<Lines> tree(xs.begin(), xs.end());
  tree.add({2, 1});
  tree.add({-1, 4});
  assert(tree.get(0) == -3);
  assert(tree.get(2) == 1);
}
```

## Notes

Determine all query coordinates before construction. Insertions can still be processed in their original chronological order.

## Related examples

- [data_structure/line_add_get_min.cpp](../../../tests/library_checker/data_structure/line_add_get_min.cpp)
