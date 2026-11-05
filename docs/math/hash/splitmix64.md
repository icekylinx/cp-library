# SplitMix64

头文件：[`lib/math/hash/splitmix64.hpp`](../../../lib/math/hash/splitmix64.hpp)。

带对象种子的 64 位整数混合函数，可用于哈希表。

## 类型与使用条件

构造时从 steady_clock 取得种子；同一对象对相同输入返回相同值。operator() 不推进状态，因此这不是逐次生成新随机数的 RNG 接口。

## 接口

| 接口 | 行为 |
| --- | --- |
| `SplitMix64()` | 初始化 seed |
| `operator()(uint64_t value)` | 返回经过种子扰动和两轮乘法混合的 64 位哈希 |
| `seed` | 可写的公开种子；需确定性复现时在首次插入哈希表之前固定 |

## 复杂度

构造、单次计算及额外空间为 O(1)。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/math/hash/splitmix64.hpp"

int main() {
  SplitMix64 hash;
  hash.seed = 123;
  assert(hash(42) == hash(42));
  assert(hash(42) != hash(43));
}
```

## 注意事项

对象建立哈希表后不能更改其 seed，否则已有键无法按原槽位查找。时钟种子不提供密码学安全性。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
