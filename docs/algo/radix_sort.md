# Integer-key radix sort

Header: [`lib/algo/radix_sort.hpp`](../../lib/algo/radix_sort.hpp).

Stably sort movable elements by unsigned integer keys. Small inputs fall back to stable_sort.

## Types and requirements

`T` is the element type; `func(element)` supplies a uint32_t or uint64_t key. Container overloads require data() and size(). Keep the default N = 16 for full-width 32-bit or 64-bit keys.

## Interface

| Interface | Behavior |
| --- | --- |
| `radix_sort_u32<T>(f, n, func)` | Sort [f, f + n) by unsigned 32-bit keys. |
| `radix_sort_u64<T>(f, n, func)` | Sort by unsigned 64-bit keys. |
| `radix_sort_u32<T>(array, func)` | Container overload for 32-bit keys. |
| `radix_sort_u64<T>(array, func)` | Container overload for 64-bit keys. |

## Complexity

With N = 16, large inputs take O(n + 2^16) time and temporary space. The small-input fallback takes O(n log n) time.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/algo/radix_sort.hpp"

int main() {
  std::vector<uint32_t> a{7, 1, 3, 1};
  radix_sort_u32<uint32_t>(a, [](uint32_t x) { return x; });
  assert((a == std::vector<uint32_t>{1, 1, 3, 7}));
}
```

## Notes

Convert signed keys to an order-preserving unsigned representation first, for example by XORing a 32-bit two's-complement key with `1u << 31`. For N < 16, the final digit is not masked, so keys must fit in 2N bits for u32 or 4N bits for u64. N must be positive. Large counting arrays are allocated on the stack.

## Related examples

- [data_structure/line_add_get_min.cpp](../../tests/library_checker/data_structure/line_add_get_min.cpp)
- [data_structure/point_set_range_composite_large_array.cpp](../../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)
- [data_structure/segment_add_get_min.cpp](../../tests/library_checker/data_structure/segment_add_get_min.cpp)
- [graph/minimum_spanning_tree.cpp](../../tests/library_checker/graph/minimum_spanning_tree.cpp)
