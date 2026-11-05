# 整数键基数排序

头文件：[`lib/algo/radix_sort.hpp`](../../lib/algo/radix_sort.hpp)。

对可移动元素按无符号整数键稳定排序。小规模输入自动回退到 stable_sort。

## 类型与使用条件

`T` 是元素类型，`func(element)` 返回 uint32_t 或 uint64_t 排序键。容器版本要求 data() 和 size()。默认 N = 16；完整 32 位或 64 位键应保持默认值。

## 接口

| 接口 | 行为 |
| --- | --- |
| `radix_sort_u32<T>(f, n, func)` | 按 32 位无符号键排序 [f, f + n) |
| `radix_sort_u64<T>(f, n, func)` | 按 64 位无符号键排序 |
| `radix_sort_u32<T>(array, func)` | 32 位容器版本 |
| `radix_sort_u64<T>(array, func)` | 64 位容器版本 |

## 复杂度

默认 N = 16 时，大规模为 O(n + 2^16) 时间、O(n + 2^16) 临时空间；小规模回退为 O(n log n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/algo/radix_sort.hpp"

int main() {
  std::vector<uint32_t> a{7, 1, 3, 1};
  radix_sort_u32<uint32_t>(a, [](uint32_t x) { return x; });
  assert((a == std::vector<uint32_t>{1, 1, 3, 7}));
}
```

## 注意事项

有符号键应先变换为保持顺序的无符号键，例如对 32 位补码键异或 `1u << 31`。N 小于 16 时，最后一段未加掩码，必须限制键值的位宽（u32 为 2N 位，u64 为 4N 位）；N 应为正数。大计数数组位于栈上。

## 相关示例

- [data_structure/line_add_get_min.cpp](../../tests/library_checker/data_structure/line_add_get_min.cpp)
- [data_structure/point_set_range_composite_large_array.cpp](../../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)
- [data_structure/segment_add_get_min.cpp](../../tests/library_checker/data_structure/segment_add_get_min.cpp)
- [graph/minimum_spanning_tree.cpp](../../tests/library_checker/graph/minimum_spanning_tree.cpp)
