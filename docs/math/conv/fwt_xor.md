# FWT XOR

头文件：[`lib/math/conv/fwt_xor.hpp`](../../../lib/math/conv/fwt_xor.hpp)。

原地执行按位异或卷积对应的变换与逆变换。用法是分别变换两个数组、逐点相乘，再逆变换。

## 类型与使用条件

`P` 是编译期模数，系数用 uint32_t 表示，输入须已归约到 [0, P)。为避免 32 位加法溢出，使用 2 <= P < 2^31。数组长度为正的 2 的幂。 XOR 逆变换还要求 P 为奇数，因而 2 在模 P 下可逆。

## 接口

| 接口 | 行为 |
| --- | --- |
| `fwt_xor<P>(array)` | 对提供 data() 和 size() 的容器进行正变换 |
| `ifwt_xor<P>(array)` | 逆变换 |
| `fwt_xor<P>(first, n) / ifwt_xor<P>(first, n)` | 迭代器和长度版本，长度会向上取整到 2 的幂 |
| `fwt_xor<P, N>(first) / ifwt_xor<P, N>(first)` | 编译期固定长度版本，N 是元素个数，不是指数 |

## 复杂度

每次变换 O(n log n) 时间、O(log n) 递归栈；逐点乘法 O(n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/conv/fwt_xor.hpp"

int main() {
  constexpr uint32_t P = 998244353;
  std::vector<uint32_t> a{1, 2}, b{3, 4};
  fwt_xor<P>(a);
  fwt_xor<P>(b);
  for (size_t i = 0; i < a.size(); ++i) a[i] = uint64_t(a[i]) * b[i] % P;
  ifwt_xor<P>(a);
  assert((a == std::vector<uint32_t>{11, 10}));
}
```

## 注意事项

动态长度入口不会分配或补零。若 n 不是 2 的幂，调用者必须预先提供向上取整后的有效存储，并自行填零；容器版本建议直接把 size 调整为该长度。默认分派支持至 2^30 个元素，超出此范围没有有效处理。

## 相关示例

- [convolution/bitwise_xor_convolution.cpp](../../../tests/library_checker/convolution/bitwise_xor_convolution.cpp)
