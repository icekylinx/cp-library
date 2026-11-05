# FastIO SIMD

头文件：[`lib/utils/fast_io_simd.hpp`](../../lib/utils/fast_io_simd.hpp)。

面向有效竞赛输入的缓冲整数和字符串读写。此版本使用 AVX2，并额外提供 GNU 128 位整数输入输出。

## 类型与使用条件

`FastIO<InputBufSize, OutputBufSize>` 需要先 init()。Unix 默认启用 mmap，输入必须为非空普通文件；从管道或终端读取时编译加 -DDISABLE_MMAP。输入必须完整、合法，接口不返回 EOF 状态。使用足够的缓冲区，例如 1 << 20 与 1 << 19。 需要支持 AVX2 的 x86 CPU。

## 接口

| 接口 | 行为 |
| --- | --- |
| `FastIO<I, O> io; io.init(input = stdin, output = stdout)` | 建立输入输出缓冲；FILE 必须在 io 销毁前保持打开 |
| `io >> value` | 跳过前导空白并读取支持的类型 |
| `io << value` | 写入整数、bool、char、C 字符串或 string |
| `io.flush()` | 显式刷新输出 |
| `fast_io::endl` | 输出换行并刷新；只输出换行时可用字符 \n |
| `io.in->read<T>() / read_small<T>()` | 底层读取，调用者负责当前位置及分隔符约定 |

## 复杂度

数字解析与输出按字符数计时，字符串处理与长度线性；缓冲模式内存 O(I + O)，mmap 输入另外映射文件大小的虚拟地址空间。

## 示例

以下示例可以独立编译运行。

<!-- runnable-example -->
```cpp
#include <cassert>
#include <cstdio>
#include <string>
#include "lib/utils/fast_io_simd.hpp"

int main() {
  FILE* input = std::tmpfile();
  FILE* output = std::tmpfile();
  assert(input && output);
  std::fputs("42 -7\n", input);
  std::rewind(input);
  {
    FastIO<4096, 4096> io;
    io.init(input, output);
    int a, b;
    io >> a >> b;
    io << a + b << '\n';
    io.flush();
  }
  std::rewind(output);
  char result[16]{};
  assert(std::fgets(result, sizeof(result), output));
  assert(std::string(result) == "35\n");
  std::fclose(input);
  std::fclose(output);
}
```

## 注意事项

fast_io.hpp 与 fast_io_simd.hpp 定义相同命名空间内的同名类型，单个翻译单元只能选择其中一个。io 不应复制或重复 init；它持有原始指针。底层 read/read_small 假定数字后有分隔符，不能代替面向任意文本或不可信输入的通用解析器。头文件中的 endl 定义也使当前实现主要适合单翻译单元的竞赛程序。

## 相关示例

- [data_structure/associative_array.cpp](../../tests/library_checker/data_structure/associative_array.cpp)
- [sample/many_a_plus_b.cpp](../../tests/library_checker/sample/many_a_plus_b.cpp)
- [sample/many_a_plus_b_128bit.cpp](../../tests/library_checker/sample/many_a_plus_b_128bit.cpp)
