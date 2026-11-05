# Binary GCD

头文件：[`lib/math/nt/binary_gcd.hpp`](../../../lib/math/nt/binary_gcd.hpp)。

用二进制移位与差值计算两个非负整数的最大公约数。

## 类型与使用条件

模板参数 T 必须是有符号整数类型，但调用值应为非负数。负值和最小有符号数不在此实现的使用条件内；差值与移位结果也必须可表示。

## 接口

| 接口 | 行为 |
| --- | --- |
| `binary_gcd(a, b)` | 返回 gcd(a, b)；一方为 0 时返回另一方，二者为 0 时返回 0 |

## 复杂度

O(log(max(a, b) + 1)) 次整数操作，空间 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/nt/binary_gcd.hpp"

int main() {
  assert(binary_gcd(48, 18) == 6);
  assert(binary_gcd(0LL, 27LL) == 27);
  assert(binary_gcd(0, 0) == 0);
}
```

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
