# LiChaoTree

头文件：[`lib/ds/li_chao_tree/li_chao_tree.hpp`](../../../lib/ds/li_chao_tree/li_chao_tree.hpp)。

在整数坐标域动态插入整条直线并查询最优值。

## 类型与使用条件

`T` 提供 Line、Value、Range、id()、range_id()、evaluate(line, x)、compare(a, b)、op(a, b)。对于最小值查询，compare 为小于、op 为 min，id 为在全坐标域内都不会优于真实直线的哨兵。直线对必须满足李超树的一次交点性质；求值和坐标运算不能溢出。 此版本的 range_id() 返回整数半开区间端点 pair{L, R}，且 L < R；查询点满足 L <= x < R。

## 接口

| 接口 | 行为 |
| --- | --- |
| `LiChaoTree() / LiChaoTree(reserved)` | 创建空树；参数只预留结点容量，不是坐标域长度 |
| `build(reserved = 0)` | 清空直线并重建根结点 |
| `reserve(size)` | 预留结点数组容量 |
| `add(line)` | 加入作用于整个坐标域的直线 |
| `get(x)` | 查询实际坐标 x 上的最优值 |

## 复杂度

设坐标域宽度 U = R - L。单次插入、查询 O(log U)，q 次插入的结点空间上界 O(q log U)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <utility>
#include "lib/ds/li_chao_tree/li_chao_tree.hpp"

struct Lines {
  using Line = std::pair<long long, long long>;
  using Value = long long;
  using Range = int;
  static Line id() { return {0, 1LL << 60}; }
  static Value evaluate(Line line, Range x) { return line.first * x + line.second; }
  static bool compare(Value a, Value b) { return a < b; }
  static Value op(Value a, Value b) { return std::min(a, b); }
  static std::pair<int, int> range_id() { return {-10, 11}; }
};
int main() {
  LiChaoTree<Lines> tree;
  tree.add({2, 1});
  tree.add({-1, 4});
  assert(tree.get(0) == 1);
  assert(tree.get(3) == 1);
}
```

## 注意事项

get 的参数是原坐标；离线版本 get 的参数则是压缩下标。此版本的 new_node 是底层接口，正常使用无需调用。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
