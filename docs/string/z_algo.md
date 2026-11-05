# Z algorithm

Header: [`lib/string/z_algo.hpp`](../../lib/string/z_algo.hpp).

Compute the longest common prefix of the entire sequence and each of its suffixes.

## Types and requirements

The input must support random access and equality comparison. The result is vector<uint32_t>. For nonempty input, z[0] = n; empty input produces an empty vector.

## Interface

| Interface | Behavior |
| --- | --- |
| `z_algo(first, n)` | Process n elements starting at first. |
| `z_algo(sequence)` | Process a container with data() and size(), such as string or vector. |

## Complexity

O(n) time and O(n) space for the result.

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <string>
#include <vector>
#include "lib/string/z_algo.hpp"

int main() {
  auto z = z_algo(std::string("ababa"));
  assert((z == std::vector<uint32_t>{5, 0, 3, 0, 1}));
  assert(z_algo(std::string{}).empty());
}
```

## Related examples

- [string/z_algorithm.cpp](../../tests/library_checker/string/z_algorithm.cpp)
