# Timer and ScopeTimer

Header: [`lib/utils/timer.hpp`](../../lib/utils/timer.hpp).

Measure elapsed milliseconds with steady_clock, optionally printing at scope exit.

## Types and requirements

Timing is always active, but print() writes to stderr only when LOCAL is defined. Timing output is kept separate from answers on stdout.

## Interface

| Interface | Behavior |
| --- | --- |
| `Timer() / Timer(name)` | Construct and start the timer. |
| `reset()` | Reset the starting time. |
| `get()` | Return elapsed milliseconds as double. |
| `print()` | Print the name and elapsed time in LOCAL mode. |
| `ScopeTimer(name)` | Automatically call the internal Timer::print() during destruction. |

## Complexity

Each operation takes O(1) time. Fixed object storage is O(1), with additional string storage proportional to the name length.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/utils/timer.hpp"

int main() {
  Timer timer("example");
  assert(timer.get() >= 0);
  timer.reset();
  { ScopeTimer scope("block"); }
}
```

## Notes

Short measurements are sensitive to caches, CPU frequency, scheduling, and I/O. Use tools/benchmark.py for complete-program benchmarking.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
