# FenwickTreeRange

Header: [`lib/ds/fenwick_tree/fenwick_tree_range.hpp`](../../../lib/ds/fenwick_tree/fenwick_tree_range.hpp).

Maintain interval increments and interval aggregates using two Fenwick trees.

## Types and requirements

`T` provides `Info`, `id()`, `op(a, b)`, and `inv(a)`, forming a commutative group: op is associative and commutative, id is its identity, and inv gives the inverse. For example, Info can be long long with addition and inv(x) = -x. Building from initial values also requires `Info{}` to equal `id()`, because the prefix helper array default-initializes its first entry. Indices are zero-based. Also provide `T::pow(v, k)` to combine v with itself k times in O(1).

## Interface

| Interface | Behavior |
| --- | --- |
| `FenwickTreeRange(n) / build(n)` | Create n identity elements. |
| `FenwickTreeRange(n, func) / (first, last)` | Build from initial values in linear time. |
| `add(l, r, v)` | Apply increment v to every position in [l, r). |
| `sum(x) / sum(l, r)` | Return a prefix or interval aggregate. |
| `reset()` | Clear both trees. |

## Complexity

Assuming O(1) group operations and pow, building and space are O(n), updates and queries take O(log n), and clearing takes O(n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/fenwick_tree/fenwick_tree_range.hpp"

struct Sum {
  using Info = long long;
  static Info id() { return 0; }
  static Info op(Info a, Info b) { return a + b; }
  static Info pow(Info x, uint32_t k) { return x * k; }
  static Info inv(Info x) { return -x; }
};
int main() {
  FenwickTreeRange<Sum> tree(5u);
  tree.add(1, 4, 3);
  assert(tree.sum(0, 5) == 9);
  assert(tree.sum(2, 4) == 6);
}
```

## Notes

Empty intervals are allowed. Products and sums in the additive example must fit their integer types.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
