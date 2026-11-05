# Fast binary search

Header: [`lib/algo/fast_bounds.hpp`](../../lib/algo/fast_bounds.hpp).

Find lower_bound or upper_bound in a sorted random-access range.

## Types and requirements

`It` must be a random-access iterator. The range is [f, f + n); n may be zero, but n + 1 must fit in uint32_t. `comp` must define a strict weak ordering consistent with the sorted range.

## Interface

| Interface | Behavior |
| --- | --- |
| `fast_lower_bound(f, n, x, comp)` | Return the first iterator for which comp(element, x) is false, or f + n if none exists. |
| `fast_upper_bound(f, n, x, comp)` | Return the first iterator for which comp(x, element) is true, or f + n if none exists. |

## Complexity

O(log(n + 1)) comparisons and O(1) extra space.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include <functional>
#include "lib/algo/fast_bounds.hpp"

int main() {
  std::vector<int> a{1, 2, 2, 5};
  auto cmp = std::less<int>{};
  assert(fast_lower_bound(a.begin(), 4, 2, cmp) == a.begin() + 1);
  assert(fast_upper_bound(a.begin(), 4, 2, cmp) == a.begin() + 3);
}
```

## Related examples

- [other/longest_increasing_subsequence.cpp](../../tests/library_checker/other/longest_increasing_subsequence.cpp)
