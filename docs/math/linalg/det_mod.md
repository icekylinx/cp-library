# 动态模数行列式

头文件：[`lib/math/linalg/det_mod.hpp`](../../../lib/math/linalg/det_mod.hpp)。

使用欧几里得式消元计算运行时模数下的行列式，支持合数模数。

## 类型与使用条件

P > 0，输入元素应已归约到 [0, P)。矩阵元素建议为 uint64_t，以容纳乘加中间值；Matrix 支持按行交换和双下标访问。

## 接口

| 接口 | 行为 |
| --- | --- |
| `det_inplace(mat, n, P)` | 原地消元并返回行列式模 P 的结果 |
| `det(mat, n, P)` | 按值复制矩阵再计算，不改变调用者输入 |

## 复杂度

欧几里得式消元为 O(n³ log P) 量级；原地版本除矩阵外 O(1) 额外空间，复制版本额外 O(n²)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/linalg/det_mod.hpp"

int main() {
  std::vector<std::vector<uint64_t>> a{{2, 1}, {1, 2}};
  auto saved = a;
  assert(det(a, 2, 12) == 3);
  assert(a == saved);
  assert(det_inplace(a, 2, 12) == 3);
  assert(det(a, 2, 1) == 0);
}
```

## 注意事项

模 1 返回 0；模数较大时应确认所有乘加均由 64 位无符号类型求值。此接口无需把模数误标记为素数。

## 相关示例

- [linear_algebra/determinant_of_matrix_arbitrary_mod.cpp](../../../tests/library_checker/linear_algebra/determinant_of_matrix_arbitrary_mod.cpp)
