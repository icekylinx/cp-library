# IntegerSet

头文件：[`lib/ds/set/integer_set.hpp`](../../../lib/ds/set/integer_set.hpp)。

使用四层 64 位位图维护有限整数集合，支持严格前驱和严格后继。

## 类型与使用条件

模板 B 默认 24；使用 0 <= B <= 24，容量上限为 2^B。构造参数 n 决定合法元素域 [0, n)，底层数组仍按 2^B 规模分配。

## 接口

| 接口 | 行为 |
| --- | --- |
| `IntegerSet<B>(n) / build(n)` | 创建或清空集合；默认构造使用 n = 2^B |
| `insert(pos) / erase(pos)` | 插入或删除，成功改变集合返回 true |
| `get(pos)` | 是否含有 pos |
| `prev(pos)` | 最大的小于 pos 的元素，不存在返回 -1 |
| `next(pos)` | 最小的大于 pos 的元素，不存在返回 -1 |
| `size()` | 由带检查插入、删除维护的元素个数 |
| `insert<false>(pos) / erase<false>(pos)` | 跳过存在性检查，返回 void，并且不维护 size 计数 |

## 复杂度

固定四层位图下单次更新、查询为 O(1)，初始化与空间为 O(2^B / 64 + 2^B / 4096 + 2^B / 262144 + 1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/set/integer_set.hpp"

int main() {
  IntegerSet<10> set(1000);
  assert(set.insert(10));
  assert(set.insert(30));
  assert(set.prev(30) == 10);
  assert(set.next(10) == 30);
  assert(set.next(30) == -1);
  assert(set.size() == 2);
}
```

## 注意事项

prev、next、get 的参数也必须满足 pos < n，不能传入 n 作为哨兵。要查询大于等于 pos 的元素，先检查 get(pos)，再调用 next(pos)。禁用检查后应自行维护集合操作的有效性，且不能依赖 size()。

## 相关示例

- [data_structure/predecessor_problem.cpp](../../../tests/library_checker/data_structure/predecessor_problem.cpp)
