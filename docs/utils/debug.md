# CHECK debug assertions

Header: [`lib/utils/debug.hpp`](../../lib/utils/debug.hpp).

Provide CHECK(expr), controlled by the LOCAL macro.

## Types and requirements

When LOCAL is defined, CHECK expands to assert(expr). Otherwise it expands to void(0), without evaluating expr. Standard assertions may also be disabled if NDEBUG is defined.

## Interface

| Interface | Behavior |
| --- | --- |
| `CHECK(expr)` | Check a condition in debug mode; do not evaluate it in release mode. |

## Complexity

No runtime cost in release mode; debug-mode cost depends on expr.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/utils/debug.hpp"

int main() {
  int x = 3;
  CHECK(x > 0);
  assert(x == 3);
}
```

## Notes

The expression must not contain required side effects, such as incrementing a counter, reading input, or applying an update.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
