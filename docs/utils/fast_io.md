# FastIO

Header: [`lib/utils/fast_io.hpp`](../../lib/utils/fast_io.hpp).

Buffered integer and string I/O for valid contest input. This version does not require AVX2 and has no dedicated 128-bit integer input or output.

## Types and requirements

`FastIO<InputBufSize, OutputBufSize>` requires init() before use. On Unix, mmap is enabled by default, so input must be a nonempty regular file. Compile with -DDISABLE_MMAP for pipes or terminal input. Input must be complete and valid; the interface does not report EOF. Use sufficiently large buffers, such as 1 << 20 and 1 << 19.

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
#include "lib/utils/fast_io.hpp"

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

- [convolution/bitwise_and_convolution.cpp](../../tests/library_checker/convolution/bitwise_and_convolution.cpp)
- [convolution/bitwise_xor_convolution.cpp](../../tests/library_checker/convolution/bitwise_xor_convolution.cpp)
- [data_structure/deque.cpp](../../tests/library_checker/data_structure/deque.cpp)
- [data_structure/line_add_get_min.cpp](../../tests/library_checker/data_structure/line_add_get_min.cpp)
- [data_structure/point_add_range_sum.cpp](../../tests/library_checker/data_structure/point_add_range_sum.cpp)
- [data_structure/point_set_range_composite.cpp](../../tests/library_checker/data_structure/point_set_range_composite.cpp)
- [data_structure/point_set_range_composite_large_array.cpp](../../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)
- [data_structure/predecessor_problem.cpp](../../tests/library_checker/data_structure/predecessor_problem.cpp)
- [data_structure/range_add_range_min.cpp](../../tests/library_checker/data_structure/range_add_range_min.cpp)
- [data_structure/range_affine_point_get.cpp](../../tests/library_checker/data_structure/range_affine_point_get.cpp)
- [data_structure/range_affine_range_sum.cpp](../../tests/library_checker/data_structure/range_affine_range_sum.cpp)
- [data_structure/segment_add_get_min.cpp](../../tests/library_checker/data_structure/segment_add_get_min.cpp)
- [data_structure/static_range_count_distinct.cpp](../../tests/library_checker/data_structure/static_range_count_distinct.cpp)
- [data_structure/static_range_frequency.cpp](../../tests/library_checker/data_structure/static_range_frequency.cpp)
- [data_structure/static_range_sum.cpp](../../tests/library_checker/data_structure/static_range_sum.cpp)
- [data_structure/static_rmq.cpp](../../tests/library_checker/data_structure/static_rmq.cpp)
- [data_structure/unionfind.cpp](../../tests/library_checker/data_structure/unionfind.cpp)
- [enumerative_combinatorics/montmort_number.cpp](../../tests/library_checker/enumerative_combinatorics/montmort_number.cpp)
- [graph/counting_spanning_trees_directed.cpp](../../tests/library_checker/graph/counting_spanning_trees_directed.cpp)
- [graph/counting_spanning_trees_undirected.cpp](../../tests/library_checker/graph/counting_spanning_trees_undirected.cpp)
- [graph/minimum_spanning_tree.cpp](../../tests/library_checker/graph/minimum_spanning_tree.cpp)
- [linear_algebra/determinant_of_matrix.cpp](../../tests/library_checker/linear_algebra/determinant_of_matrix.cpp)
- [linear_algebra/determinant_of_matrix_arbitrary_mod.cpp](../../tests/library_checker/linear_algebra/determinant_of_matrix_arbitrary_mod.cpp)
- [other/longest_increasing_subsequence.cpp](../../tests/library_checker/other/longest_increasing_subsequence.cpp)
- [string/z_algorithm.cpp](../../tests/library_checker/string/z_algorithm.cpp)
- [tree/lowest_common_ancestor.cpp](../../tests/library_checker/tree/lowest_common_ancestor.cpp)
- [tree/tree_diameter.cpp](../../tests/library_checker/tree/tree_diameter.cpp)

## Further reading

- [Input modes and compilation flags](../usage.md)
