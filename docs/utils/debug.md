# CHECK 调试断言

头文件：[`lib/utils/debug.hpp`](../../lib/utils/debug.hpp)。

提供受 LOCAL 宏控制的 CHECK(expr)。

## 类型与使用条件

在编译时定义 LOCAL 才展开为 assert(expr)；未定义 LOCAL 时展开为 void(0)，表达式不会被求值。若同时定义 NDEBUG，标准 assert 也可能被禁用。

## 接口

| 接口 | 行为 |
| --- | --- |
| `CHECK(expr)` | 调试模式验证条件，正式模式不求值 |

## 复杂度

正式模式无运行成本；调试模式取决于 expr。

## 示例

以下示例可以独立编译运行。

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

## 注意事项

表达式不能带必需的副作用，如递增计数、读取输入或执行实际更新。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
