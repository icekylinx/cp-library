# 快速二分查找

头文件：[`lib/algo/fast_bounds.hpp`](../../lib/algo/fast_bounds.hpp)。

在已按比较器排序的随机访问范围中定位 lower_bound 或 upper_bound。

## 类型与使用条件

`It` 为随机访问迭代器；范围为 [f, f + n)，n 可为 0，但 n + 1 必须能由 uint32_t 表示。`comp` 是与排序一致的严格弱序。

## 接口

| 接口 | 行为 |
| --- | --- |
| `fast_lower_bound(f, n, x, comp)` | 返回首个不满足 comp(element, x) 的迭代器，未找到返回 f + n |
| `fast_upper_bound(f, n, x, comp)` | 返回首个满足 comp(x, element) 的迭代器，未找到返回 f + n |

## 复杂度

O(log(n + 1)) 次比较，额外空间 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include <functional>
#include "lib/algo/fast_bounds.hpp"

int main() {
  std::vector<int> a{1, 2, 2, 5};
  auto cmp = std::less<int>{};
  assert(fast_lower_bound(a.begin(), 4, 2, cmp) == a.begin() + 1);
  assert(fast_upper_bound(a.begin(), 4, 2, cmp) == a.begin() + 3);
}
```

## 相关示例

- [other/longest_increasing_subsequence.cpp](../../tests/library_checker/other/longest_increasing_subsequence.cpp)
