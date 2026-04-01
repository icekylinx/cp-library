Header: `lib/ds/seg_tree/seg_tree.hpp`

# SegTree

`SegTree<T>` maintains interval summaries for a monoid `T`.

It supports:

- point assignment
- range monoid information query
- `evaluate`, which folds the information on `[l, r)` into an external value

## Requirements

Provide a type `T` satisfying:

```cpp
struct T {
  using Info = ...;

  static Info id();
  static Info op(const Info& lhs, const Info& rhs);
};
```

where:

- `Info` is the summary type for an interval.
- `id()` returns the identity element of `op`.
- `op(lhs, rhs)` combines the summaries of two adjacent intervals in left-to-right order.
- `op` is associative in that order.

Equivalently, `(Info, op, id)` forms a monoid, not necessarily commutative.

## Interface

```cpp
template <typename T>
struct SegTree {
  using Info = typename T::Info;

  SegTree();
  explicit SegTree(uint32_t m);

  template <typename F>
  SegTree(uint32_t m, F&& func);

  template <typename It>
  SegTree(It first, It last);

  void build(uint32_t m);

  template <typename F>
  void build(uint32_t m, F&& func);

  template <typename It>
  void build(It first, It last);

  void reset();

  void set(uint32_t x, const Info& v);
  Info get(uint32_t x) const;

  Info prod(uint32_t l, uint32_t r) const;
  Info all_prod() const;

  template <typename Value, typename F>
  Value evaluate(uint32_t l, uint32_t r, Value init, F&& func) const;
};
```

## Complexity

- `construct / build`: $O(n)$
- `reset`: $O(n)$
- `set`: $O(\log n)$
- `get`: $O(1)$
- `prod`: $O(\log n)$
- `all_prod`: $O(1)$
- `evaluate`: $O(\log n)$

## Constructors

### `SegTree(uint32_t m)`

Builds a `SegTree` of length `m`, initialized with `T::id()`.

### `SegTree(uint32_t m, F&& func)`

Builds a `SegTree` of length `m`, where the information at position `i` is `func(i)`.

### `SegTree(It first, It last)`

Builds a `SegTree` from an iterator range `[first, last)`.

## Operations

### `reset()`

Resets the whole tree to `T::id()`.

### `set(uint32_t x, const Info& v)`

Assigns the information at position `x` to `v`.

### `get(uint32_t x) const`

Returns the current information at position `x`.

### `prod(uint32_t l, uint32_t r) const`

Returns the monoid information on `[l, r)`.

If `l == r`, this returns `T::id()`.

### `all_prod() const`

Returns the monoid information on the whole interval.

### `evaluate(uint32_t l, uint32_t r, Value init, F&& func) const`

Folds the monoid information on `[l, r)` into `init` from left to right.

This is useful when the query result is not naturally another `Info`.

`func` should satisfy the following:

- it can be called as `func(value, info)`
- `value` has type `Value`
- `info` has type `Info`
- it returns the updated value, again of type `Value`

## Preconditions

The caller must ensure:

- all indices are 0-indexed
- all intervals are half-open: `[l, r)`
- `0 <= x < m` for `get` and `set`
- `0 <= l <= r <= m` for `prod` and `evaluate`

Also note:

- this library assumes `uint32_t` is large enough to store sizes
- `size == 0` may lead to unexpected behavior
- only limited safety checks are performed

## Example

```cpp
#include "lib/ds/seg_tree/seg_tree.hpp"

constexpr uint32_t P = 998244353;

struct AffineMonoid {
  using Info = std::pair<uint32_t, uint32_t>;  // f(x) = a * x + b

  static constexpr Info id() noexcept {
    return {1, 0};
  }

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {
      1ull * lhs.first * rhs.first % P,
      (1ull * lhs.second * rhs.first + rhs.second) % P
    };
  }
};

uint32_t apply(uint32_t x, const AffineMonoid::Info& f) {
  return (1ull * x * f.first + f.second) % P;
}

void example(std::vector<std::pair<uint32_t, uint32_t>> a) {
  SegTree<AffineMonoid> seg(a.begin(), a.end());

  seg.set(3, {5, 7});

  auto f = seg.prod(1, 4);

  uint32_t x = 10;
  uint32_t y = seg.evaluate(1, 4, x, [&](uint32_t cur, const auto& info) {
    return apply(cur, info);
  });
}
```

## Notes

Use `SegTree` when point assignment and interval monoid query are enough.

If you need range updates, use `LazySegTree` or `TagSegTree` instead.