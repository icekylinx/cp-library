# TagSegTree

头文件：[`lib/ds/seg_tree/tag_seg_tree.hpp`](../../../lib/ds/seg_tree/tag_seg_tree.hpp)。

维护区间更新与单点查询，适合不需要区间聚合的场景。

## 类型与使用条件

`T` 提供 Info、Tag、id()、tag_id()、apply(Info&, const Tag&)、compose(Tag&, const Tag&)；无需 op。compose(old, newer) 表示先应用 old，再应用 newer。tag_id 必须是恒等标记。

## 接口

| 接口 | 行为 |
| --- | --- |
| `TagSegTree(n) / (n, func) / (first, last)` | 初始化单点值 |
| `build(...) / reset()` | 重建或恢复初始单位元 |
| `apply(l, r, tag)` | 对 [l, r) 追加变换 |
| `set(x, value)` | 覆盖单点值，并消除对该点尚未落地的标记 |
| `get(x)` | 返回单点经过全部标记后的值 |
| `get_ref(x)` | 下推该点路径上的标记，返回底层值的可写引用 |

## 复杂度

建树、清空、空间 O(n)，区间更新及单点 set、get、get_ref 为 O(log n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/seg_tree/tag_seg_tree.hpp"

struct Model {
  using Info = long long;
  struct Tag { long long mul, add; };
  static Info id() { return 0; }
  static Tag tag_id() { return {1, 0}; }
  static void apply(Info& x, Tag t) { x = x * t.mul + t.add; }
  static void compose(Tag& old, Tag newer) {
    old = {old.mul * newer.mul, old.add * newer.mul + newer.add};
  }
};
int main() {
  TagSegTree<Model> tree(3u, [](uint32_t i) { return i + 1; });
  tree.apply(0, 3, {2, 0});
  tree.apply(1, 3, {1, 5});
  assert(tree.get(1) == 9);
  tree.get_ref(1) += 1;
  assert(tree.get(1) == 10);
  tree.set(1, 7);
  assert(tree.get(1) == 7);
}
```

## 注意事项

get_ref 返回的引用不会自动跟踪后续尚未下推的更新；再次读取当前逻辑值应调用 get 或 get_ref。不要跨 build 保存引用。

## 相关示例

- [data_structure/range_affine_point_get.cpp](../../../tests/library_checker/data_structure/range_affine_point_get.cpp)
