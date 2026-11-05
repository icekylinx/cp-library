# FenwickTree

Header: [`lib/ds/fenwick_tree/fenwick_tree.hpp`](../../../lib/ds/fenwick_tree/fenwick_tree.hpp).

Maintain point increments and prefix or interval aggregates.

## Types and requirements

`T` provides `Info`, `id()`, `op(a, b)`, and `inv(a)`, forming a commutative group: op is associative and commutative, id is its identity, and inv gives the inverse. For example, Info can be long long with addition and inv(x) = -x. Building from initial values also requires `Info{}` to equal `id()`, because the prefix helper array default-initializes its first entry. Indices are zero-based.

## Interface

| Interface | Behavior |
| --- | --- |
| `FenwickTree(n) / build(n)` | Create n identity elements. |
| `FenwickTree(n, func) / FenwickTree(first, last)` | Build in linear time from a generator or iterator range. |
| `add(x, v)` | Combine the value at x with v. |
| `sum(x)` | Aggregate [0, x), allowing x = 0 or n. |
| `sum(l, r)` | Aggregate [l, r). |
| `reset()` | Reset every value to the identity. |

## Complexity

Building, clearing, and space are O(n). Updates and prefix or interval queries take O(log n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/fenwick_tree/fenwick_tree.hpp"

struct Sum {
  using Info = long long;
  static Info id() { return 0; }
  static Info op(Info a, Info b) { return a + b; }
  static Info inv(Info x) { return -x; }
};
int main() {
  FenwickTree<Sum> tree(4u, [](uint32_t i) { return i + 1; });
  tree.add(1, 5);
  assert(tree.sum(0, 3) == 11);
  tree.reset();
  assert(tree.sum(4) == 0);
}
```

## Notes

Intervals must satisfy `0 <= l <= r <= n`. `add` applies an increment rather than an assignment; compute the difference first when assigning a new value.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.

## Further reading

- [Choosing a range structure](../../guides/data_structures.md)
- [Commutative groups](../../guides/algebra.md)
