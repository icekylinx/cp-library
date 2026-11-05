# 固定模数行列式

头文件：[`lib/math/linalg/det_fixed_mod.hpp`](../../../lib/math/linalg/det_fixed_mod.hpp)。

计算编译期模数 P 下的方阵行列式。prime = true 使用逆元消元及 AVX2；prime = false 使用欧几里得式消元。

## 类型与使用条件

`Matrix` 支持 mat[row][column]。建议使用 vector<vector<uint64_t>>，并将元素归约到 [0, P)。P > 0，prime = true 时 P 必须为素数。AVX2 分支使用有符号 32 位乘法及 64 位累加，应满足 P < 2^31 且 N * (P - 1)^2 + (P - 1) <= UINT64_MAX；默认 N = 16 适用于 998244353。

## 接口

| 接口 | 行为 |
| --- | --- |
| `det_inplace<P, N = 16, prime = true>(mat, n)` | 原地计算，破坏矩阵 |
| `det<P, N = 16, prime = true>(mat, n)` | 复制为 64 位矩阵后计算，保留输入 |

## 复杂度

素数分支 O(n³ + n² log P) 时间，另用 O(n) 计数空间；合数分支 O(n³ log P) 量级。det 包装器额外复制 O(n²) 时间与空间。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <vector>
#include "lib/math/linalg/det_fixed_mod.hpp"

int main() {
  std::vector<std::vector<uint64_t>> a{{1, 2}, {3, 4}};
  auto saved = a;
  assert(det<998244353>(a, 2) == 998244351);
  assert(a == saved);
  assert(det_inplace<998244353>(a, 2) == 998244351);
  std::vector<std::vector<uint64_t>> b{{2, 1}, {1, 2}};
  assert((det<12, 16, false>(b, 2) == 3));
}
```

## 注意事项

文件启用 GCC target("avx2")，需要兼容的 x86 CPU。prime = true 的原地接口必须使用 64 位元素，SIMD 每次按 64 位槽位读写；不要直接传 uint32_t 矩阵。N 必须为正。

## 相关示例

- [graph/counting_spanning_trees_directed.cpp](../../../tests/library_checker/graph/counting_spanning_trees_directed.cpp)
- [graph/counting_spanning_trees_undirected.cpp](../../../tests/library_checker/graph/counting_spanning_trees_undirected.cpp)
- [linear_algebra/determinant_of_matrix.cpp](../../../tests/library_checker/linear_algebra/determinant_of_matrix.cpp)
