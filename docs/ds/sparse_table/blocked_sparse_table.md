# BlockedSparseTable

头文件：[`lib/ds/sparse_table/blocked_sparse_table.hpp`](../../../lib/ds/sparse_table/blocked_sparse_table.hpp)。

维护静态区间的幂等聚合，如最小值、最大值或 gcd。按 B 个元素分块，默认 B = 16。

## 类型与使用条件

`T` 提供 Info、id()、op(a, b)。op 必须结合且幂等，因为查询可能重复覆盖元素；幂等要求 op(x, x) = x。区间为 [l, r)，空区间返回 id()。

## 接口

| 接口 | 行为 |
| --- | --- |
| `BlockedSparseTable(first, last)` | 从迭代器范围建表 |
| `BlockedSparseTable(n, func)` | 通过 func(i) 生成初值 |
| `build(first, last) / build(n, func)` | 重建静态表 |
| `prod(l, r)` | 查询区间聚合，不支持更新 |

## 复杂度

预处理与空间 O(n + (n/B) log(n/B))；同块查询 O(B)，跨块查询 O(1)。B 固定为 16 时查询为常数级。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <limits>
#include <vector>
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"

struct Min {
  using Info = int;
  static Info id() { return std::numeric_limits<int>::max(); }
  static Info op(Info a, Info b) { return std::min(a, b); }
};
int main() {
  std::vector<int> a{5, 2, 8, 1};
  BlockedSparseTable<Min> table(a.begin(), a.end());
  assert(table.prod(0, 3) == 2);
  assert(table.prod(2, 4) == 1);
}
```

## 注意事项

不能直接用于普通区间和；区间和不满足幂等性。 B 必须大于 0。

## 相关示例

- [data_structure/static_rmq.cpp](../../../tests/library_checker/data_structure/static_rmq.cpp)
- [tree/lowest_common_ancestor.cpp](../../../tests/library_checker/tree/lowest_common_ancestor.cpp)
