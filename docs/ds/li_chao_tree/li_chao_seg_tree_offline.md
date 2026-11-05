# LiChaoSegTreeOffline

头文件：[`lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp`](../../../lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp)。

预先收集查询坐标后的李超树。支持只在指定坐标区间生效的线段。

## 类型与使用条件

`T` 提供 Line、Value、Range、id()、range_id()、evaluate(line, x)、compare(a, b)、op(a, b)。对于最小值查询，compare 为小于、op 为 min，id 为在全坐标域内都不会优于真实直线的哨兵。直线对必须满足李超树的一次交点性质；求值和坐标运算不能溢出。 此版本的 range_id() 返回单个坐标哨兵，须不小于全部查询坐标；哨兵处求值也必须安全。建议至少提供一个查询点。

## 接口

| 接口 | 行为 |
| --- | --- |
| `LiChaoSegTreeOffline(first, last) / build(first, last)` | 传入非降序排列的全部查询坐标；保留重复坐标 |
| `get(i)` | 查询第 i 个坐标的最优值，参数是下标而非原坐标 |
| `add_seg(l, r, line)` | 仅在压缩下标区间 [l, r) 加入直线 |
| `add_line(line)` | 在全部查询坐标加入直线 |

## 复杂度

设查询坐标数为 m，初始化与空间 O(m)，查询 O(log m)。整线插入 O(log m)，线段插入 O(log² m)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <utility>
#include <vector>
#include "lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp"

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
  LiChaoSegTreeOffline<Lines> tree(xs.begin(), xs.end());
  tree.add_line({2, 1});
  tree.add_seg(1, 3, {-1, 4});
  assert(tree.get(0) == -3);
  assert(tree.get(2) == 1);
}
```

## 注意事项

若要插入原坐标区间 [a, b) 上的线段，应先在坐标数组中用 lower_bound 分别求出 l、r，再调用 add_seg。

## 相关示例

- [data_structure/segment_add_get_min.cpp](../../../tests/library_checker/data_structure/segment_add_get_min.cpp)
