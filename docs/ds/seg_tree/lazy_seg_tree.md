# LazySegTree

Header: [`lib/ds/seg_tree/lazy_seg_tree.hpp`](../../../lib/ds/seg_tree/lazy_seg_tree.hpp).

Maintain interval updates and interval aggregates with composable lazy tags.

## Types and requirements

`T` provides Info, Tag, id(), tag_id(), op(info, info), apply(Info&, const Tag&), and compose(Tag&, const Tag&). Info and op form a monoid, and tag actions must be compatible with op. compose(old, newer) combines the tags in the order old followed by newer. tag_id must act as the identity. Store any required interval length in Info.

## Interface

| Interface | Behavior |
| --- | --- |
| `LazySegTree(n) / (n, func) / (first, last)` | Build from identity values, a generator, or iterators. |
| `build(...) / reset()` | Rebuild, or reset values and tags to their identities. |
| `set(x, info)` | Overwrite the value at one position. |
| `get(x)` | Read the value at one position. |
| `apply(l, r, tag)` | Apply a tag to [l, r). |
| `apply_all(tag)` | Apply a tag to the whole tree, including padded leaves. |
| `prod(l, r) / all_prod()` | Return an interval aggregate or the root aggregate. |

## Complexity

Assuming O(1) model operations, building, clearing, and space are O(n). set, get, apply, and prod take O(log n); apply_all and all_prod take O(1).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/seg_tree/lazy_seg_tree.hpp"

struct Model {
  struct Info { long long sum, len; };
  struct Tag { long long mul, add; };
  static Info id() { return {0, 0}; }
  static Tag tag_id() { return {1, 0}; }
  static Info op(Info a, Info b) { return {a.sum + b.sum, a.len + b.len}; }
  static void apply(Info& a, Tag t) { a.sum = a.sum * t.mul + a.len * t.add; }
  static void compose(Tag& old, Tag newer) {
    old = {old.mul * newer.mul, old.add * newer.mul + newer.add};
  }
};
int main() {
  LazySegTree<Model> tree(3u, [](uint32_t i) { return Model::Info{i + 1, 1}; });
  tree.apply(0, 3, {2, 1});
  assert(tree.prod(0, 3).sum == 15);
  tree.apply(1, 3, {1, 4});
  assert(tree.get(1).sum == 9);
  tree.set(0, {10, 1});
  assert(tree.all_prod().sum == 30);
}
```

## Notes

Require x < n and 0 <= l <= r <= n; empty intervals return id(). The leaf count is padded to a power of two. When using apply_all, tags must leave padded identity values unchanged. Call build before using a default-constructed tree.

## Related examples

- [data_structure/range_affine_range_sum.cpp](../../../tests/library_checker/data_structure/range_affine_range_sum.cpp)
