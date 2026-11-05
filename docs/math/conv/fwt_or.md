# FWT OR

Header: [`lib/math/conv/fwt_or.hpp`](../../../lib/math/conv/fwt_or.hpp).

Apply the forward or inverse transform for bitwise OR convolution in place. Transform both inputs, multiply pointwise, then apply the inverse transform.

## Types and requirements

`P` is a compile-time modulus. Coefficients are uint32_t values already reduced to [0, P). Use 2 <= P < 2^31 to avoid overflow in 32-bit addition. The array length must be a positive power of two.

## Interface

| Interface | Behavior |
| --- | --- |
| `fwt_or<P>(array)` | Apply the forward transform to a container providing data() and size(). |
| `ifwt_or<P>(array)` | Apply the inverse transform. |
| `fwt_or<P>(first, n) / ifwt_or<P>(first, n)` | Iterator-and-length overloads; the length is rounded up to a power of two. |
| `fwt_or<P, N>(first) / ifwt_or<P, N>(first)` | Compile-time fixed-length overloads; N is the element count, not its exponent. |

## Complexity

Each transform takes O(n log n) time and O(log n) recursion stack space. Pointwise multiplication takes O(n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/conv/fwt_or.hpp"

int main() {
  constexpr uint32_t P = 998244353;
  std::vector<uint32_t> a{1, 2}, b{3, 4};
  fwt_or<P>(a);
  fwt_or<P>(b);
  for (size_t i = 0; i < a.size(); ++i) a[i] = uint64_t(a[i]) * b[i] % P;
  ifwt_or<P>(a);
  assert((a == std::vector<uint32_t>{3, 18}));
}
```

## Notes

The dynamic-length entry point does not allocate or zero-pad storage. If n is not a power of two, provide valid storage up to the rounded length and zero-pad it yourself. For container overloads, resize the container to that length first. The default dispatcher supports up to 2^30 elements; larger sizes are not handled.

## Related examples

No existing Library Checker program directly includes this header. The example above checks only the usage shown.
