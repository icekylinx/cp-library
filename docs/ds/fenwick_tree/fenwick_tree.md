# FenwickTree

头文件：[`lib/ds/fenwick_tree/fenwick_tree.hpp`](../../../lib/ds/fenwick_tree/fenwick_tree.hpp)。

维护单点增量与前缀、区间聚合。

## 类型与使用条件

`T` 提供 `Info`、`id()`、`op(a, b)`、`inv(a)`。这里按交换群使用：`op` 结合且交换，`id` 为单位元，`inv` 为逆元。例如 `Info = long long`，`op` 为加法，`inv(x) = -x`。从初值建树时还要求 `Info{}` 等于 `id()`，因为前缀辅助数组的第 0 项使用默认初始化。下标从 0 开始。

## 接口

| 接口 | 行为 |
| --- | --- |
| `Class(n) / build(n)` | 创建 n 个单位元 |
| `Class(n, func) / Class(first, last)` | 从生成函数或迭代器范围线性建树；Class 指本页类型 |
| `add(x, v)` | 将位置 x 与 v 合并 |
| `sum(x)` | 聚合 [0, x)，允许 x = 0 或 n |
| `sum(l, r)` | 聚合 [l, r) |
| `reset()` | 全部恢复单位元 |

## 复杂度

建树、清空和空间 O(n)；更新、前缀和及区间查询 O(log n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/fenwick_tree/fenwick_tree.hpp"

struct Sum {
  using Info = long long;
  static Info id() { return 0; }
  static Info op(Info a, Info b) { return a + b; }
  static Info inv(Info x) { return -x; }
};
int main() {
  FenwickTree<Sum> tree(4u, [](uint32_t i) { return i + 1; });
  tree.add(1, 5);
  assert(tree.sum(0, 3) == 11);
  tree.reset();
  assert(tree.sum(4) == 0);
}
```

## 注意事项

区间满足 `0 <= l <= r <= n`。`add` 是增量，不是赋值；赋值需先计算差值。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
