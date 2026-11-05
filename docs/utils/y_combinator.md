# Y Combinator

头文件：[`lib/utils/y_combinator.hpp`](../../lib/utils/y_combinator.hpp)。

把接收自身引用的 lambda 包装为可递归调用对象。

## 类型与使用条件

lambda 的第一个参数接收自身，后续参数为普通函数参数。调用运算符为 const；闭包需可在 const 上调用。递归 lambda 通常应显式指定返回类型。

## 接口

| 接口 | 行为 |
| --- | --- |
| `y_combinator(lambda)` | 衰减并保存可调用对象，返回递归包装器 |
| `wrapper(args...)` | 等价于调用保存的函数 func(self, args...) |

## 复杂度

包装调用额外开销 O(1)，整体时间与递归栈取决于算法；包装对象空间等于保存闭包的大小量级。

## 示例

以下示例可以独立编译运行。

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

## 注意事项

不改变递归深度上限；对很深的链式递归仍应考虑显式栈。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
