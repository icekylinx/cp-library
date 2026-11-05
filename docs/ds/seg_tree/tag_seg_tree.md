# TagSegTree

Header: [`lib/ds/seg_tree/tag_seg_tree.hpp`](../../../lib/ds/seg_tree/tag_seg_tree.hpp).

Maintain interval updates and point queries when interval aggregation is unnecessary.

## Types and requirements

`T` provides Info, Tag, id(), tag_id(), apply(Info&, const Tag&), and compose(Tag&, const Tag&); op is not needed. compose(old, newer) means applying old first, then newer. tag_id must be an identity tag.

## Interface

| Interface | Behavior |
| --- | --- |
| `TagSegTree(n) / (n, func) / (first, last)` | Initialize point values. |
| `build(...) / reset()` | Rebuild or reset values and tags to their identities. |
| `apply(l, r, tag)` | Append a transformation to [l, r). |
| `set(x, value)` | Overwrite a point and discard pending tags affecting its previous value. |
| `get(x)` | Return a point value after all pending tags. |
| `get_ref(x)` | Push tags along the point path and return a writable reference to its stored value. |

## Complexity

Building, clearing, and space are O(n). Interval updates and point set, get, and get_ref take O(log n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/seg_tree/tag_seg_tree.hpp"

struct Model {
  using Info = long long;
  struct Tag { long long mul, add; };
  static Info id() { return 0; }
  static Tag tag_id() { return {1, 0}; }
  static void apply(Info& x, Tag t) { x = x * t.mul + t.add; }
  static void compose(Tag& old, Tag newer) {
    old = {old.mul * newer.mul, old.add * newer.mul + newer.add};
  }
};
int main() {
  TagSegTree<Model> tree(3u, [](uint32_t i) { return i + 1; });
  tree.apply(0, 3, {2, 0});
  tree.apply(1, 3, {1, 5});
  assert(tree.get(1) == 9);
  tree.get_ref(1) += 1;
  assert(tree.get(1) == 10);
  tree.set(1, 7);
  assert(tree.get(1) == 7);
}
```

## Notes

A reference returned by get_ref does not automatically reflect later updates that have not been pushed down. Call get or get_ref again to read the current logical value. Do not retain references across build.

## Related examples

- [data_structure/range_affine_point_get.cpp](../../../tests/library_checker/data_structure/range_affine_point_get.cpp)
