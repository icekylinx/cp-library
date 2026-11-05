# FWT AND

Header: [`lib/math/conv/fwt_and.hpp`](../../../lib/math/conv/fwt_and.hpp).

Apply the forward or inverse transform for bitwise AND convolution in place. Transform both inputs, multiply pointwise, then apply the inverse transform.

## Types and requirements

`P` is a compile-time modulus. Coefficients are uint32_t values already reduced to [0, P). Use 2 <= P < 2^31 to avoid overflow in 32-bit addition. The array length must be a positive power of two.

## Interface

| Interface | Behavior |
| --- | --- |
| `fwt_and<P>(array)` | Apply the forward transform to a container providing data() and size(). |
| `ifwt_and<P>(array)` | Apply the inverse transform. |
| `fwt_and<P>(first, n) / ifwt_and<P>(first, n)` | Iterator-and-length overloads; the length is rounded up to a power of two. |
| `fwt_and<P, N>(first) / ifwt_and<P, N>(first)` | Compile-time fixed-length overloads; N is the element count, not its exponent. |

## Complexity

Each transform takes O(n log n) time and O(log n) recursion stack space. Pointwise multiplication takes O(n).

## Example

This example can be compiled and run as a standalone program.

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/conv/fwt_and.hpp"

int main() {
  constexpr uint32_t P = 998244353;
  std::vector<uint32_t> a{1, 2}, b{3, 4};
  fwt_and<P>(a);
  fwt_and<P>(b);
  for (size_t i = 0; i < a.size(); ++i) a[i] = uint64_t(a[i]) * b[i] % P;
  ifwt_and<P>(a);
  assert((a == std::vector<uint32_t>{13, 8}));
}
```

## Notes

The dynamic-length entry point does not allocate or zero-pad storage. If n is not a power of two, provide valid storage up to the rounded length and zero-pad it yourself. For container overloads, resize the container to that length first. The default dispatcher supports up to 2^30 elements; larger sizes are not handled.

## Related examples

- [convolution/bitwise_and_convolution.cpp](../../../tests/library_checker/convolution/bitwise_and_convolution.cpp)
