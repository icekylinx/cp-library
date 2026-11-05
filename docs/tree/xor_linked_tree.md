# XorLinkedTree

头文件：[`lib/tree/xor_linked_tree.hpp`](../../lib/tree/xor_linked_tree.hpp)。

用度数与邻点异或和存储树，以剥叶方式按子到父的顺序处理每条边。

## 类型与使用条件

输入必须为无自环、连通的树。weighted 为 false 时不保存权值；为 true 时 T 需支持异或运算。build 会破坏内部的度数与邻点数据。

## 接口

| 接口 | 行为 |
| --- | --- |
| `XorLinkedTree(n) / init(n)` | 创建 n 个点并清空边 |
| `add_edge(u, v)` | 无权版本加入边 |
| `add_edge(u, v, w)` | 有权版本加入权值为 w 的边 |
| `build(root, callback)` | 无权调用 callback(child, parent)，有权调用 callback(child, parent, weight)；根也会触发一次回调，调用者必须跳过 child == root 的情形 |

## 复杂度

初始化 O(n)，每次加边 O(1)，一次完整 build 为 O(n)，空间 O(n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <algorithm>
#include <vector>
#include "lib/tree/xor_linked_tree.hpp"

int main() {
  XorLinkedTree<true, int> tree(3);
  tree.add_edge(0, 1, 4);
  tree.add_edge(1, 2, 7);
  std::vector<int> dist(3, 0);
  int edges = 0;
  tree.build(0, [&](uint32_t child, uint32_t parent, int weight) {
    if (child == 0) return;  // 忽略根的回调
    dist[parent] = std::max(dist[parent], dist[child] + weight);
    ++edges;
  });
  assert(edges == 2 && dist[0] == 11);
}
```

## 注意事项

根回调中的 parent 是内部哨兵 0，不代表树中的父节点；应在访问父节点或累计边数前跳过根。适合树形 DP、自底向上汇总；不提供一般邻居遍历。重复处理前必须 init 并重新加入全部边。

## 相关示例

- [tree/tree_diameter.cpp](../../tests/library_checker/tree/tree_diameter.cpp)
