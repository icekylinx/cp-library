# IntegerSet

Header: [`lib/ds/set/integer_set.hpp`](../../../lib/ds/set/integer_set.hpp).

Maintain a bounded set of integers using four levels of 64-bit bitmaps, with strict predecessor and successor queries.

## Types and requirements

B defaults to 24; use 0 <= B <= 24, giving capacity 2^B. The constructor argument n defines the valid element domain [0, n), but internal arrays are still sized for 2^B.

## Interface

| Interface | Behavior |
| --- | --- |
| `IntegerSet<B>(n) / build(n)` | Create or clear the set; default construction uses n = 2^B. |
| `insert(pos) / erase(pos)` | Insert or erase an element; return true if the set changed. |
| `get(pos)` | Check whether pos is present. |
| `prev(pos)` | Return the largest element strictly less than pos, or -1 if none exists. |
| `next(pos)` | Return the smallest element strictly greater than pos, or -1 if none exists. |
| `size()` | Return the count maintained by checked insertion and deletion. |
| `insert<false>(pos) / erase<false>(pos)` | Skip existence checks, return void, and do not maintain the size counter. |

## Complexity

With four fixed bitmap levels, each update or query takes O(1). Initialization and space are O(2^B / 64 + 2^B / 4096 + 2^B / 262144 + 1).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/set/integer_set.hpp"

int main() {
  IntegerSet<10> set(1000);
  assert(set.insert(10));
  assert(set.insert(30));
  assert(set.prev(30) == 10);
  assert(set.next(10) == 30);
  assert(set.next(30) == -1);
  assert(set.size() == 2);
}
```

## Notes

prev, next, and get also require pos < n; n is not a valid sentinel argument. For an inclusive successor query, check get(pos) before next(pos). With checks disabled, maintain operation validity yourself and do not rely on size().

## Related examples

- [data_structure/predecessor_problem.cpp](../../../tests/library_checker/data_structure/predecessor_problem.cpp)
