# Algebra and tag composition

The library separates storage and traversal from the operations applied to values. Define the model laws first; the headers perform only limited runtime checks and cannot verify these laws.

## Monoids and ordered aggregation

A monoid supplies Info, id(), and op(a, b), with these laws:

- Associativity: `op(op(a, b), c) == op(a, op(b, c))`.
- Identity: `op(id(), a) == a` and `op(a, id()) == a`.

SegTree preserves the left-to-right order of its interval. For an affine function f(x) = a*x + b, combining the left function f and right function g means applying f first and then g. The result has coefficients `(g.a * f.a, g.a * f.b + g.b)`. Swapping the functions changes the answer in general.

Do not substitute arithmetic sum for a problem that needs function composition, or assume an aggregate can be accumulated in either order.

## Commutative groups and repetition

The full Fenwick interfaces use an inverse to subtract a prefix. Their expected model is a commutative group: a monoid with op(a, b) == op(b, a), and an inverse satisfying op(a, inv(a)) == id().

For integer sums, use id() = 0, op(a, b) = a + b, and inv(a) = -a. Unsigned sums can instead operate in modular wraparound arithmetic when that is the intended problem model.

FenwickTreeRange additionally calls pow(value, k), meaning repeated combination k times. For addition, pow(value, k) = value * k. It must agree with the group operation and run in constant time for the advertised bounds. The initial-value builders currently default-initialize a helper prefix entry, so they also require Info{} == id().

## Idempotent operations

Sparse table queries may overlap precomputed blocks. They require op(a, a) == a in addition to associativity. Minimum, maximum, and gcd satisfy this property; ordinary sums do not.

The identity must work with padded elements and empty intervals. For minimum over signed integers, the largest representable integer is a common identity. Avoid performing arithmetic on that sentinel unless overflow is explicitly handled.

## Actions and chronological composition

LazySegTree and TagSegTree call compose(old, newer) with the meaning "apply old, then newer." For affine tags x -> mul*x + add, update the old tag as follows:

```cpp
old = {newer.mul * old.mul,
       newer.mul * old.add + newer.add};
```

The identity tag is `{1, 0}`. Multiplying by 2 and then adding 3 maps x to 2*x + 3. Adding 3 and then multiplying by 2 maps x to 2*x + 6. This distinction is a useful first check of composition order.

For LazySegTree range sums, Info should store both sum and length. Applying a tag to an aggregate becomes `sum = mul * sum + add * len`. The empty aggregate is `{0, 0}` so that padding remains unchanged, even under apply_all.

The following example uses two noncommuting updates and also checks an element outside their intersection.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/seg_tree/tag_seg_tree.hpp"

struct Affine {
  using Info = long long;
  struct Tag { long long mul, add; };
  static Info id() { return 0; }
  static Tag tag_id() { return {1, 0}; }
  static void apply(Info& x, const Tag& t) { x = t.mul * x + t.add; }
  static void compose(Tag& old, const Tag& newer) {
    old = {newer.mul * old.mul, newer.mul * old.add + newer.add};
  }
};

int main() {
  TagSegTree<Affine> tree(3u, [](uint32_t) { return 5LL; });
  tree.apply(0, 3, {2, 0});
  tree.apply(1, 3, {1, 3});
  assert(tree.get(0) == 10);
  assert(tree.get(1) == 13);
  tree.apply(1, 2, {2, 0});
  assert(tree.get(1) == 26);
  assert(tree.get(2) == 13);
}
```

## Arithmetic limits

These equations describe exact arithmetic. C++ signed overflow is undefined, and modular models need sufficiently wide intermediate types before reducing. The existing [range-affine example](../../tests/library_checker/data_structure/range_affine_range_sum.cpp) uses uint64_t intermediate products modulo 998244353.

The module pages separately document narrower restrictions of optimized implementations, including [Barrett modulus ranges](../math/modular/barrett64.md), [fixed-modulus determinant accumulation](../math/linalg/det_fixed_mod.md), and [radix key widths](../algo/radix_sort.md).
