# LiChaoTreeOffline

头文件：[`lib/ds/li_chao_tree/li_chao_tree_offline.hpp`](../../../lib/ds/li_chao_tree/li_chao_tree_offline.hpp)。

预先收集查询坐标后的李超树。支持整条直线插入。

## 类型与使用条件

`T` 提供 Line、Value、Range、id()、range_id()、evaluate(line, x)、compare(a, b)、op(a, b)。对于最小值查询，compare 为小于、op 为 min，id 为在全坐标域内都不会优于真实直线的哨兵。直线对必须满足李超树的一次交点性质；求值和坐标运算不能溢出。 此版本的 range_id() 返回单个坐标哨兵，须不小于全部查询坐标；哨兵处求值也必须安全。建议至少提供一个查询点。

## 接口

| 接口 | 行为 |
| --- | --- |
| `LiChaoTreeOffline(first, last) / build(first, last)` | 传入非降序排列的全部查询坐标；保留重复坐标 |
| `get(i)` | 查询第 i 个坐标的最优值，参数是下标而非原坐标 |
| `add(line)` | 在全部查询坐标加入直线 |

## 复杂度

设查询坐标数为 m，初始化与空间 O(m)，查询 O(log m)。整线插入 O(log m)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <utility>
#include <vector>
#include "lib/ds/li_chao_tree/li_chao_tree_offline.hpp"

struct Lines {
  using Line = std::pair<long long, long long>;
  using Value = long long;
  using Range = int;
  static Line id() { return {0, 1LL << 60}; }
  static Value evaluate(Line line, Range x) { return line.first * x + line.second; }
  static bool compare(Value a, Value b) { return a < b; }
  static Value op(Value a, Value b) { return std::min(a, b); }
  static int range_id() { return 100; }
};
int main() {
  std::vector<int> xs{-2, 0, 3};
  LiChaoTreeOffline<Lines> tree(xs.begin(), xs.end());
  tree.add({2, 1});
  tree.add({-1, 4});
  assert(tree.get(0) == -3);
  assert(tree.get(2) == 1);
}
```

## 注意事项

所有查询坐标必须在建树前确定；插入操作仍可按原时间顺序执行。

## 相关示例

- [data_structure/line_add_get_min.cpp](../../../tests/library_checker/data_structure/line_add_get_min.cpp)
