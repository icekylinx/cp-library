# Timer 与 ScopeTimer

头文件：[`lib/utils/timer.hpp`](../../lib/utils/timer.hpp)。

使用 steady_clock 测量经过的毫秒数，支持作用域结束时打印。

## 类型与使用条件

计时始终启用；print() 仅在 LOCAL 模式向 stderr 输出。输出不会与 stdout 中的答案混合。

## 接口

| 接口 | 行为 |
| --- | --- |
| `Timer() / Timer(name)` | 创建并开始计时 |
| `reset()` | 重新记录起点 |
| `get()` | 返回自起点以来的毫秒数，类型 double |
| `print()` | LOCAL 模式输出名称与耗时 |
| `ScopeTimer(name)` | 析构时自动调用内部 Timer::print() |

## 复杂度

每次操作 O(1)，对象空间 O(1)，名称字符串的存储与名称长度有关。

## 示例

以下示例可以独立编译运行。

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

## 注意事项

局部计时容易受缓存、CPU 频率、任务调度及 I/O 影响；本库完整程序 benchmark 使用独立的 tools/benchmark.py。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
