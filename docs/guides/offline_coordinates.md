# Offline query coordinates

The offline Li Chao variants build their storage from known query coordinates. Insertions can still be processed chronologically; only the locations at which answers will be requested must be collected in advance.

## Coordinate values and indices

1. Collect every queried x-coordinate.
2. Sort the coordinates. Deduplicate if one stored index per distinct coordinate is convenient; the implementations also accept duplicate coordinates.
3. Build the offline tree from the sorted range.
4. Call get(i) with a compressed index, not an original x-coordinate.

For [LiChaoSegTreeOffline](../ds/li_chao_tree/li_chao_seg_tree_offline.md), a segment active on original coordinates [a, b) corresponds to `[lower_bound(xs, a), lower_bound(xs, b))`. The right endpoint is excluded. Empty compressed intervals can be passed to add_seg; the operation returns without insertion.

For [LiChaoTreeOffline](../ds/li_chao_tree/li_chao_tree_offline.md), add(line) inserts a whole line. The segment variant names the corresponding operation add_line(line). The [online tree](../ds/li_chao_tree/li_chao_tree.md) instead accepts actual coordinates in get and obtains a pair of domain endpoints from range_id().

## Sentinel requirements

In the offline variants, range_id() returns one coordinate sentinel at least as large as every stored coordinate. The arrays pad to a power of two and may evaluate lines at that sentinel, even though it is not itself queried. Choose coefficient and coordinate types so every such evaluation fits Value.

For a minimum query, id() must be a line that never beats a real line, including at the sentinel. A constant infinity line is convenient only when its value exceeds every legitimate result and its evaluation cannot overflow. Avoid constructing a sentinel as max_coordinate + 1 when that addition might overflow; choose a wider Range type if necessary.

## Restricted-line example

The line y = 2*x + 1 is active only on [-1, 8). The background line y = 50 remains active everywhere. Coordinate 8 is excluded from the segment.

<!-- runnable-example -->
```cpp
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>
#include "lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp"

struct MinimumLine {
  using Line = std::pair<long long, long long>;
  using Range = int;
  using Value = long long;
  static Line id() { return {0, 1LL << 60}; }
  static Range range_id() { return 100; }
  static Value evaluate(Line f, Range x) { return f.first * x + f.second; }
  static bool compare(Value a, Value b) { return a < b; }
  static Value op(Value a, Value b) { return std::min(a, b); }
};

int main() {
  std::vector<int> xs{-5, 0, 3, 8};
  LiChaoSegTreeOffline<MinimumLine> tree(xs.begin(), xs.end());
  auto index = [&](int x) -> uint32_t {
    return std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
  };
  tree.add_line({0, 50});
  tree.add_seg(index(-1), index(8), {2, 1});
  assert(tree.get(0) == 50);
  assert(tree.get(1) == 1);
  assert(tree.get(2) == 7);
  assert(tree.get(3) == 50);
}
```

## Sorting packed events

The existing [line-add program](../../tests/library_checker/data_structure/line_add_get_min.cpp) stores a query identifier and a transformed coordinate in a uint64_t, then sorts by its low 32 bits. Converting a signed 32-bit coordinate with an offset of 2^31 preserves its order in unsigned arithmetic.

For [radix_sort](../algo/radix_sort.md), use the default N = 16 for full-width keys. A smaller N is valid only when the problem bounds keep the final digit within its counting array. The large-array composition program uses N = 15 because its coordinates fit in 30 bits. Check the actual problem bound before changing that parameter.
