# DSU

头文件：[`lib/ds/dsu/dsu.hpp`](../../../lib/ds/dsu/dsu.hpp)。

维护集合合并与连通性，采用路径压缩及按集合大小合并。

## 类型与使用条件

点编号为 [0, n)。默认构造后需调用 resize；resize 会清空旧集合。

## 接口

| 接口 | 行为 |
| --- | --- |
| `DSU(n) / resize(n)` | 创建 n 个单元素集合 |
| `find(x)` | 返回代表元并压缩路径 |
| `merge(x, y)` | 合并两个集合，成功合并返回 true，原本同集合返回 false |
| `same(x, y)` | 判断是否同集合 |
| `size(x)` | 返回 x 所在集合的大小 |

## 复杂度

初始化与空间 O(n)；find、merge、same、size 的均摊复杂度为 O(α(n))。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/dsu/dsu.hpp"

int main() {
  DSU dsu(4);
  assert(dsu.merge(0, 1));
  assert(!dsu.merge(1, 0));
  assert(dsu.same(0, 1));
  assert(dsu.size(1) == 2);
}
```

## 注意事项

merge 后代表元可能改变；此实现不支持拆分或回滚。

## 相关示例

- [data_structure/unionfind.cpp](../../../tests/library_checker/data_structure/unionfind.cpp)
- [graph/minimum_spanning_tree.cpp](../../../tests/library_checker/graph/minimum_spanning_tree.cpp)
