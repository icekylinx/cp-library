# Barrett32

头文件：[`lib/math/modular/barrett32.hpp`](../../../lib/math/modular/barrett32.hpp)。

对 32 位无符号整数使用预计算倒数进行固定模数取余。

## 类型与使用条件

先通过构造函数或 set_mod 设置模数。此实现通过最高位判断负余数修正，使用安全范围 2 <= mod <= 2^31；不要把仅有的 CHECK(mod >= 2) 理解为支持任意满位宽模数。

## 接口

| 接口 | 行为 |
| --- | --- |
| `Barrett32(mod)` | 设置模数与倒数 |
| `set_mod(mod)` | 切换模数 |
| `reduce(uint32_t x)` | 返回 x % mod，x 可以使用整个无符号位宽 |

## 复杂度

设置模数包含一次整数除法，reduce 为固定次数乘法及移位，均为 O(1)；空间 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <limits>
#include "lib/math/modular/barrett32.hpp"

int main() {
  Barrett32 reduce(998244353);
  uint32_t x = std::numeric_limits<uint32_t>::max();
  assert(reduce.reduce(x) == x % 998244353);
  reduce.set_mod(17);
  assert(reduce.reduce(12345) == 3);
}
```

## 注意事项

本版本输入只有 32 位；若需计算两个 32 位数的乘积模 m，应使用 64 位取余器或普通 uint64_t 取余。 默认构造不初始化 mod、inv，不能直接 reduce。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
