# FastIO SIMD

Header: [`lib/utils/fast_io_simd.hpp`](../../lib/utils/fast_io_simd.hpp).

Buffered integer and string I/O for valid contest input. This version uses AVX2 and additionally supports GNU 128-bit integer input and output.

## Types and requirements

`FastIO<InputBufSize, OutputBufSize>` requires init() before use. On Unix, mmap is enabled by default, so input must be a nonempty regular file. Compile with -DDISABLE_MMAP for pipes or terminal input. Input must be complete and valid; the interface does not report EOF. Use sufficiently large buffers, such as 1 << 20 and 1 << 19. An x86 CPU with AVX2 support is required.

## Interface

| Interface | Behavior |
| --- | --- |
| `FastIO<I, O> io; io.init(input = stdin, output = stdout)` | Initialize input and output buffers; the FILE objects must remain open until io is destroyed. |
| `io >> value` | Skip leading whitespace and read a supported type. |
| `io << value` | Write integers, bool, char, C strings, or string. |
| `io.flush()` | Explicitly flush output. |
| `fast_io::endl` | Write a newline and flush; use the newline character alone when flushing is unnecessary. |
| `io.in->read<T>() / read_small<T>()` | Low-level reading; the caller must satisfy cursor and delimiter conventions. |

## Complexity

Number parsing and formatting take time proportional to character count; string operations are linear in string length. Buffered mode uses O(I + O) memory. mmap additionally maps the input file into virtual address space.

## Example

This example can be compiled and run as a standalone program.

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

## Notes

fast_io.hpp and fast_io_simd.hpp define the same types in the same namespace; include only one in a translation unit. Do not copy io or call init repeatedly, since it owns raw pointers. Low-level read/read_small require delimiters after numbers and are not general parsers for arbitrary or untrusted input. The header-level definition of endl also makes this implementation primarily suitable for single-translation-unit contest programs.

## Related examples

- [data_structure/associative_array.cpp](../../tests/library_checker/data_structure/associative_array.cpp)
- [sample/many_a_plus_b.cpp](../../tests/library_checker/sample/many_a_plus_b.cpp)
- [sample/many_a_plus_b_128bit.cpp](../../tests/library_checker/sample/many_a_plus_b_128bit.cpp)

## Further reading

- [Input modes and compilation flags](../usage.md)
