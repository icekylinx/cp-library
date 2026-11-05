# SafeHashMap

头文件：[`lib/ds/hash_map/safe_hash_map.hpp`](../../../lib/ds/hash_map/safe_hash_map.hpp)。

固定 2^N 个槽位的哈希映射。HashMapCore 配合 SplitMix64 的别名，哈希种子随对象初始化。

## 类型与使用条件

`0 < N < 32`，键支持比较，值可以默认构造。表不会扩容，必须始终保留至少一个空槽，否则缺失查询或新键插入可能无法终止。 键应能转换为 uint64_t。

## 接口

| 接口 | 行为 |
| --- | --- |
| `operator[](key)` | 查找或插入；新值由 V{} 初始化，返回可写引用 |
| `get(key)` | 按值返回；缺失时返回 V{}，不会插入 |
| `count(key)` | 判断键是否存在 |
| `clear()` | 清空占用标记，旧值在重新插入时初始化 |
| `size()` | 当前不同键的个数 |

## 复杂度

在哈希分布良好且负载率受控时，查找与插入期望 O(1)，最坏 O(2^N)，空间 O(2^N)。 clear 在已插入元素不超过容量的 1/128 时只清理记录过的槽位；否则重置整个位图。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include "lib/ds/hash_map/safe_hash_map.hpp"

int main() {
  static SafeHashMap<uint64_t, int, 10> map;
  map[42] = 7;
  assert(map.get(42) == 7);
  assert(map.get(99) == 0);
  assert(!map.count(99));
  map.clear();
  assert(!map.count(42));
}
```

## 注意事项

对象含内嵌数组，N 较大时建议放在静态存储区或堆上；不支持删除单个键或迭代。 “Safe” 指带随机种子的哈希，不能用作密码学安全保证。

## 相关示例

现有 Library Checker 程序未直接包含此头文件；本页示例只验证所示用法。
