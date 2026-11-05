# FastHasher

头文件：[`lib/math/hash/fast_hasher.hpp`](../../../lib/math/hash/fast_hasher.hpp)。

将整数键乘以固定奇常数，生成 64 位无符号哈希值。

## 类型与使用条件

`T` 能显式转换为 uint64_t。此哈希是确定性的，主要配合取高位的固定容量哈希表使用。

## 接口

| 接口 | 行为 |
| --- | --- |
| `FastHasher<T>{}(value)` | 返回 Magic * uint64_t(value)，按 uint64_t 自然溢出 |
| `FastHasher<T>::Magic` | 固定乘数 11400714819323198485 |

## 复杂度

单次调用时间、额外空间均为 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/hash/fast_hasher.hpp"

int main() {
  FastHasher<uint64_t> hash;
  assert(hash(0) == 0);
  assert(hash(1) == FastHasher<uint64_t>::Magic);
}
```

## 注意事项

不提供随机化或密码学抗碰撞保证。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
