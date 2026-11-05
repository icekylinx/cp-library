# Y Combinator

Header: [`lib/utils/y_combinator.hpp`](../../lib/utils/y_combinator.hpp).

Wrap a lambda that accepts a reference to itself into a recursively callable object.

## Types and requirements

The first lambda parameter receives the recursive wrapper; subsequent parameters are ordinary arguments. The wrapper's call operator is const, so the closure must be const-callable. Recursive lambdas generally need an explicit return type.

## Interface

| Interface | Behavior |
| --- | --- |
| `y_combinator(lambda)` | Decay and store the callable, returning a recursive wrapper. |
| `wrapper(args...)` | Invoke the stored function as func(self, args...). |

## Complexity

The wrapper adds O(1) work per call; total time and recursion-stack usage depend on the algorithm. Object space is proportional to the stored closure size.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/utils/y_combinator.hpp"

int main() {
  auto factorial = y_combinator([](auto self, int n) -> long long {
    return n <= 1 ? 1 : n * self(n - 1);
  });
  assert(factorial(10) == 3628800);
}
```

## Notes

The wrapper does not increase the recursion-depth limit. Consider an explicit stack for very deep recursion.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
