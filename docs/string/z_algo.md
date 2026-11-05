# Z 算法

头文件：[`lib/string/z_algo.hpp`](../../lib/string/z_algo.hpp)。

计算每个后缀与整个序列的最长公共前缀长度。

## 类型与使用条件

输入为支持随机访问和相等比较的序列。返回 vector<uint32_t>；非空输入满足 z[0] = n，空输入返回空向量。

## 接口

| 接口 | 行为 |
| --- | --- |
| `z_algo(first, n)` | 处理从 first 开始的 n 个元素 |
| `z_algo(sequence)` | 处理提供 data() 与 size() 的容器，例如 string、vector |

## 复杂度

时间 O(n)，返回数组空间 O(n)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <string>
#include <vector>
#include "lib/string/z_algo.hpp"

int main() {
  auto z = z_algo(std::string("ababa"));
  assert((z == std::vector<uint32_t>{5, 0, 3, 0, 1}));
  assert(z_algo(std::string{}).empty());
}
```

## 相关示例

- [string/z_algorithm.cpp](../../tests/library_checker/string/z_algorithm.cpp)
