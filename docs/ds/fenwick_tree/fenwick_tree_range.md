# FenwickTreeRange

头文件：[`lib/ds/fenwick_tree/fenwick_tree_range.hpp`](../../../lib/ds/fenwick_tree/fenwick_tree_range.hpp)。

通过两棵树状数组维护区间增量与区间聚合。

## 类型与使用条件

`T` 提供 `Info`、`id()`、`op(a, b)`、`inv(a)`。这里按交换群使用：`op` 结合且交换，`id` 为单位元，`inv` 为逆元。例如 `Info = long long`，`op` 为加法，`inv(x) = -x`。从初值建树时还要求 `Info{}` 等于 `id()`，因为前缀辅助数组的第 0 项使用默认初始化。下标从 0 开始。 另外需要 `T::pow(v, k)` 表示将 v 合并 k 次；该操作应能在 O(1) 内完成。

## 接口

| 接口 | 行为 |
| --- | --- |
| `FenwickTreeRange(n) / build(n)` | 创建 n 个单位元 |
| `FenwickTreeRange(n, func) / (first, last)` | 从初值线性建树 |
| `add(l, r, v)` | 在 [l, r) 的每个位置施加增量 v |
| `sum(x) / sum(l, r)` | 前缀或区间聚合 |
| `reset()` | 清空两棵树 |

## 复杂度

假设群操作与 pow 为 O(1)，建树和空间 O(n)，更新与查询 O(log n)，清空 O(n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/fenwick_tree/fenwick_tree_range.hpp"

struct Sum {
  using Info = long long;
  static Info id() { return 0; }
  static Info op(Info a, Info b) { return a + b; }
  static Info pow(Info x, uint32_t k) { return x * k; }
  static Info inv(Info x) { return -x; }
};
int main() {
  FenwickTreeRange<Sum> tree(5u);
  tree.add(1, 4, 3);
  assert(tree.sum(0, 5) == 9);
  assert(tree.sum(2, 4) == 6);
}
```

## 注意事项

区间允许为空；加法示例中的乘积与和需保证不溢出。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
