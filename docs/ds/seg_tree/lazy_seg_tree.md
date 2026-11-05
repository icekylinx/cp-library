# LazySegTree

头文件：[`lib/ds/seg_tree/lazy_seg_tree.hpp`](../../../lib/ds/seg_tree/lazy_seg_tree.hpp)。

维护区间更新与区间聚合，使用可组合的懒标记。

## 类型与使用条件

`T` 提供 Info、Tag、id()、tag_id()、op(info, info)、apply(Info&, const Tag&) 和 compose(Tag&, const Tag&)。Info 与 op 构成幺半群；标记作用须与 op 相容。compose(old, newer) 将 newer 合并到 old，顺序是先 old 后 newer。tag_id 对 Info 的作用必须为恒等。区间长度等信息应由 Info 自行携带。

## 接口

| 接口 | 行为 |
| --- | --- |
| `LazySegTree(n) / (n, func) / (first, last)` | 以单位元、函数或迭代器构建 |
| `build(...) / reset()` | 重建或恢复单位元及空标记 |
| `set(x, info)` | 把单点覆盖为 info |
| `get(x)` | 读取单点信息 |
| `apply(l, r, tag)` | 在 [l, r) 施加标记 |
| `apply_all(tag)` | 对整棵树施加标记，包括补齐叶子 |
| `prod(l, r) / all_prod()` | 区间聚合或根结点聚合 |

## 复杂度

若模型操作 O(1)，建树、清空、空间 O(n)，set、get、apply、prod 为 O(log n)，apply_all 和 all_prod 为 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/seg_tree/lazy_seg_tree.hpp"

struct Model {
  struct Info { long long sum, len; };
  struct Tag { long long mul, add; };
  static Info id() { return {0, 0}; }
  static Tag tag_id() { return {1, 0}; }
  static Info op(Info a, Info b) { return {a.sum + b.sum, a.len + b.len}; }
  static void apply(Info& a, Tag t) { a.sum = a.sum * t.mul + a.len * t.add; }
  static void compose(Tag& old, Tag newer) {
    old = {old.mul * newer.mul, old.add * newer.mul + newer.add};
  }
};
int main() {
  LazySegTree<Model> tree(3u, [](uint32_t i) { return Model::Info{i + 1, 1}; });
  tree.apply(0, 3, {2, 1});
  assert(tree.prod(0, 3).sum == 15);
  tree.apply(1, 3, {1, 4});
  assert(tree.get(1).sum == 9);
  tree.set(0, {10, 1});
  assert(tree.all_prod().sum == 30);
}
```

## 注意事项

下标 x < n，区间 0 <= l <= r <= n，空区间返回 id()。实现会将叶子数补为 2 的幂；使用 apply_all 时应保证标记不改变补齐部分的单位元。默认构造后先 build。

## 相关示例

- [data_structure/range_affine_range_sum.cpp](../../../tests/library_checker/data_structure/range_affine_range_sum.cpp)
