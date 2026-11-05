# 父数组转 DFS 编号

头文件：[`lib/tree/parent_to_dfn.hpp`](../../lib/tree/parent_to_dfn.hpp)。

在线性时间内把特定编号约束下的父数组转换为 DFS 前序编号。

## 类型与使用条件

节点编号为 [0, n)，根为 0，且每个 i >= 1 都满足 fa[i] < i。fa[0] 不参与计算。返回 dfn[i] 是节点 i 的前序位置；根位置为 0。

## 接口

| 接口 | 行为 |
| --- | --- |
| `parent_to_dfn(first, n)` | 读取 n 个父编号，返回前序位置数组 |
| `parent_to_dfn(parent)` | 容器版本，要求 data() 与 size() |

## 复杂度

时间 O(n)，额外空间 O(n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <vector>
#include "lib/tree/parent_to_dfn.hpp"

int main() {
  std::vector<uint32_t> parent{0, 0, 0, 1};
  auto dfn = parent_to_dfn(parent);
  assert(dfn[0] == 0);
  assert(dfn[1] + 1 == dfn[3]);
  std::sort(dfn.begin(), dfn.end());
  assert((dfn == std::vector<uint32_t>{0, 1, 2, 3}));
}
```

## 注意事项

兄弟子树的访问顺序由实现决定，不应假定为邻接表顺序。返回的是节点到位置的映射，不是按访问顺序排列的节点列表；实现不返回子树大小或子树终点。

## 相关示例

- [tree/lowest_common_ancestor.cpp](../../tests/library_checker/tree/lowest_common_ancestor.cpp)
