# 算法库目录

共 38 个头文件、38 篇模块文档、37 个 Sublime Text 代码片段、31 个 Library Checker 程序。示例栏列出直接包含头文件的程序，不代表完整正确性覆盖。

## algo

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [fast_bounds.hpp](../lib/algo/fast_bounds.hpp) | [文档](algo/fast_bounds.md) | [片段](../sublime_snippets/algo/fast_bounds.sublime-snippet) | [other/longest_increasing_subsequence.cpp](../tests/library_checker/other/longest_increasing_subsequence.cpp) |
| [radix_sort.hpp](../lib/algo/radix_sort.hpp) | [文档](algo/radix_sort.md) | [片段](../sublime_snippets/algo/radix_sort.sublime-snippet) | [data_structure/line_add_get_min.cpp](../tests/library_checker/data_structure/line_add_get_min.cpp)<br>[data_structure/point_set_range_composite_large_array.cpp](../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)<br>[data_structure/segment_add_get_min.cpp](../tests/library_checker/data_structure/segment_add_get_min.cpp)<br>[graph/minimum_spanning_tree.cpp](../tests/library_checker/graph/minimum_spanning_tree.cpp) |

## ds/dsu

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [dsu.hpp](../lib/ds/dsu/dsu.hpp) | [文档](ds/dsu/dsu.md) | [片段](../sublime_snippets/ds/dsu/dsu.sublime-snippet) | [data_structure/unionfind.cpp](../tests/library_checker/data_structure/unionfind.cpp)<br>[graph/minimum_spanning_tree.cpp](../tests/library_checker/graph/minimum_spanning_tree.cpp) |

## ds/fenwick_tree

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [fast_fenwick_tree.hpp](../lib/ds/fenwick_tree/fast_fenwick_tree.hpp) | [文档](ds/fenwick_tree/fast_fenwick_tree.md) | [片段](../sublime_snippets/ds/fenwick_tree/fast_fenwick_tree.sublime-snippet) | [data_structure/point_add_range_sum.cpp](../tests/library_checker/data_structure/point_add_range_sum.cpp)<br>[data_structure/static_range_count_distinct.cpp](../tests/library_checker/data_structure/static_range_count_distinct.cpp) |
| [fenwick_tree.hpp](../lib/ds/fenwick_tree/fenwick_tree.hpp) | [文档](ds/fenwick_tree/fenwick_tree.md) | [片段](../sublime_snippets/ds/fenwick_tree/fenwick_tree.sublime-snippet) | — |
| [fenwick_tree_range.hpp](../lib/ds/fenwick_tree/fenwick_tree_range.hpp) | [文档](ds/fenwick_tree/fenwick_tree_range.md) | [片段](../sublime_snippets/ds/fenwick_tree/fenwick_tree_range.sublime-snippet) | — |

## ds/hash_map

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [fast_hash_map.hpp](../lib/ds/hash_map/fast_hash_map.hpp) | [文档](ds/hash_map/fast_hash_map.md) | [片段](../sublime_snippets/ds/hash_map/fast_hash_map.sublime-snippet) | [data_structure/associative_array.cpp](../tests/library_checker/data_structure/associative_array.cpp)<br>[data_structure/static_range_count_distinct.cpp](../tests/library_checker/data_structure/static_range_count_distinct.cpp)<br>[data_structure/static_range_frequency.cpp](../tests/library_checker/data_structure/static_range_frequency.cpp) |
| [hash_map.hpp](../lib/ds/hash_map/hash_map.hpp) | [文档](ds/hash_map/hash_map.md) | [片段](../sublime_snippets/ds/hash_map/hash_map.sublime-snippet) | — |
| [hash_map_core.hpp](../lib/ds/hash_map/hash_map_core.hpp) | [文档](ds/hash_map/hash_map_core.md) | [片段](../sublime_snippets/ds/hash_map/hash_map_core.sublime-snippet) | — |
| [safe_hash_map.hpp](../lib/ds/hash_map/safe_hash_map.hpp) | [文档](ds/hash_map/safe_hash_map.md) | [片段](../sublime_snippets/ds/hash_map/safe_hash_map.sublime-snippet) | — |

## ds/li_chao_tree

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [li_chao_seg_tree_offline.hpp](../lib/ds/li_chao_tree/li_chao_seg_tree_offline.hpp) | [文档](ds/li_chao_tree/li_chao_seg_tree_offline.md) | [片段](../sublime_snippets/ds/li_chao_tree/li_chao_seg_tree_offline.sublime-snippet) | [data_structure/segment_add_get_min.cpp](../tests/library_checker/data_structure/segment_add_get_min.cpp) |
| [li_chao_tree.hpp](../lib/ds/li_chao_tree/li_chao_tree.hpp) | [文档](ds/li_chao_tree/li_chao_tree.md) | [片段](../sublime_snippets/ds/li_chao_tree/li_chao_tree.sublime-snippet) | — |
| [li_chao_tree_offline.hpp](../lib/ds/li_chao_tree/li_chao_tree_offline.hpp) | [文档](ds/li_chao_tree/li_chao_tree_offline.md) | [片段](../sublime_snippets/ds/li_chao_tree/li_chao_tree_offline.sublime-snippet) | [data_structure/line_add_get_min.cpp](../tests/library_checker/data_structure/line_add_get_min.cpp) |

## ds/seg_tree

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [lazy_seg_tree.hpp](../lib/ds/seg_tree/lazy_seg_tree.hpp) | [文档](ds/seg_tree/lazy_seg_tree.md) | [片段](../sublime_snippets/ds/seg_tree/lazy_seg_tree.sublime-snippet) | [data_structure/range_affine_range_sum.cpp](../tests/library_checker/data_structure/range_affine_range_sum.cpp) |
| [seg_tree.hpp](../lib/ds/seg_tree/seg_tree.hpp) | [文档](ds/seg_tree/seg_tree.md) | [片段](../sublime_snippets/ds/seg_tree/seg_tree.sublime-snippet) | [data_structure/point_set_range_composite.cpp](../tests/library_checker/data_structure/point_set_range_composite.cpp)<br>[data_structure/point_set_range_composite_large_array.cpp](../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)<br>[data_structure/range_add_range_min.cpp](../tests/library_checker/data_structure/range_add_range_min.cpp) |
| [tag_seg_tree.hpp](../lib/ds/seg_tree/tag_seg_tree.hpp) | [文档](ds/seg_tree/tag_seg_tree.md) | [片段](../sublime_snippets/ds/seg_tree/tag_seg_tree.sublime-snippet) | [data_structure/range_affine_point_get.cpp](../tests/library_checker/data_structure/range_affine_point_get.cpp) |

## ds/set

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [integer_set.hpp](../lib/ds/set/integer_set.hpp) | [文档](ds/set/integer_set.md) | [片段](../sublime_snippets/ds/set/integer_set.sublime-snippet) | [data_structure/predecessor_problem.cpp](../tests/library_checker/data_structure/predecessor_problem.cpp) |

## ds/sparse_table

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [blocked_sparse_table.hpp](../lib/ds/sparse_table/blocked_sparse_table.hpp) | [文档](ds/sparse_table/blocked_sparse_table.md) | [片段](../sublime_snippets/ds/sparse_table/blocked_sparse_table.sublime-snippet) | [data_structure/static_rmq.cpp](../tests/library_checker/data_structure/static_rmq.cpp)<br>[tree/lowest_common_ancestor.cpp](../tests/library_checker/tree/lowest_common_ancestor.cpp) |
| [sparse_table.hpp](../lib/ds/sparse_table/sparse_table.hpp) | [文档](ds/sparse_table/sparse_table.md) | [片段](../sublime_snippets/ds/sparse_table/sparse_table.sublime-snippet) | — |

## math/conv

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [fwt_and.hpp](../lib/math/conv/fwt_and.hpp) | [文档](math/conv/fwt_and.md) | [片段](../sublime_snippets/math/conv/fwt_and.sublime-snippet) | [convolution/bitwise_and_convolution.cpp](../tests/library_checker/convolution/bitwise_and_convolution.cpp) |
| [fwt_or.hpp](../lib/math/conv/fwt_or.hpp) | [文档](math/conv/fwt_or.md) | [片段](../sublime_snippets/math/conv/fwt_or.sublime-snippet) | — |
| [fwt_xor.hpp](../lib/math/conv/fwt_xor.hpp) | [文档](math/conv/fwt_xor.md) | [片段](../sublime_snippets/math/conv/fwt_xor.sublime-snippet) | [convolution/bitwise_xor_convolution.cpp](../tests/library_checker/convolution/bitwise_xor_convolution.cpp) |

## math/hash

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [fast_hasher.hpp](../lib/math/hash/fast_hasher.hpp) | [文档](math/hash/fast_hasher.md) | [片段](../sublime_snippets/math/hash/fast_hasher.sublime-snippet) | — |
| [splitmix64.hpp](../lib/math/hash/splitmix64.hpp) | [文档](math/hash/splitmix64.md) | [片段](../sublime_snippets/math/hash/splitmix64.sublime-snippet) | — |

## math/linalg

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [det_fixed_mod.hpp](../lib/math/linalg/det_fixed_mod.hpp) | [文档](math/linalg/det_fixed_mod.md) | [片段](../sublime_snippets/math/linalg/det_fixed_mod.sublime-snippet) | [graph/counting_spanning_trees_directed.cpp](../tests/library_checker/graph/counting_spanning_trees_directed.cpp)<br>[graph/counting_spanning_trees_undirected.cpp](../tests/library_checker/graph/counting_spanning_trees_undirected.cpp)<br>[linear_algebra/determinant_of_matrix.cpp](../tests/library_checker/linear_algebra/determinant_of_matrix.cpp) |
| [det_mod.hpp](../lib/math/linalg/det_mod.hpp) | [文档](math/linalg/det_mod.md) | [片段](../sublime_snippets/math/linalg/det_mod.sublime-snippet) | [linear_algebra/determinant_of_matrix_arbitrary_mod.cpp](../tests/library_checker/linear_algebra/determinant_of_matrix_arbitrary_mod.cpp) |

## math/modular

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [barrett32.hpp](../lib/math/modular/barrett32.hpp) | [文档](math/modular/barrett32.md) | [片段](../sublime_snippets/math/modular/barrett32.sublime-snippet) | — |
| [barrett64.hpp](../lib/math/modular/barrett64.hpp) | [文档](math/modular/barrett64.md) | [片段](../sublime_snippets/math/modular/barrett64.sublime-snippet) | [enumerative_combinatorics/montmort_number.cpp](../tests/library_checker/enumerative_combinatorics/montmort_number.cpp) |
| [online_inverse.hpp](../lib/math/modular/online_inverse.hpp) | [文档](math/modular/online_inverse.md) | — | — |

## math/nt

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [binary_gcd.hpp](../lib/math/nt/binary_gcd.hpp) | [文档](math/nt/binary_gcd.md) | [片段](../sublime_snippets/math/nt/binary_gcd.sublime-snippet) | — |

## string

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [z_algo.hpp](../lib/string/z_algo.hpp) | [文档](string/z_algo.md) | [片段](../sublime_snippets/string/z_algo.sublime-snippet) | [string/z_algorithm.cpp](../tests/library_checker/string/z_algorithm.cpp) |

## tree

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [parent_to_dfn.hpp](../lib/tree/parent_to_dfn.hpp) | [文档](tree/parent_to_dfn.md) | [片段](../sublime_snippets/tree/parent_to_dfn.sublime-snippet) | [tree/lowest_common_ancestor.cpp](../tests/library_checker/tree/lowest_common_ancestor.cpp) |
| [xor_linked_tree.hpp](../lib/tree/xor_linked_tree.hpp) | [文档](tree/xor_linked_tree.md) | [片段](../sublime_snippets/tree/xor_linked_tree.sublime-snippet) | [tree/tree_diameter.cpp](../tests/library_checker/tree/tree_diameter.cpp) |

## utils

| 头文件 | 文档 | 代码片段 | 直接引用示例 |
| --- | --- | --- | --- |
| [debug.hpp](../lib/utils/debug.hpp) | [文档](utils/debug.md) | [片段](../sublime_snippets/utils/debug.sublime-snippet) | — |
| [fast_io.hpp](../lib/utils/fast_io.hpp) | [文档](utils/fast_io.md) | [片段](../sublime_snippets/utils/fast_io.sublime-snippet) | [convolution/bitwise_and_convolution.cpp](../tests/library_checker/convolution/bitwise_and_convolution.cpp)<br>[convolution/bitwise_xor_convolution.cpp](../tests/library_checker/convolution/bitwise_xor_convolution.cpp)<br>[data_structure/deque.cpp](../tests/library_checker/data_structure/deque.cpp)<br>[data_structure/line_add_get_min.cpp](../tests/library_checker/data_structure/line_add_get_min.cpp)<br>[data_structure/point_add_range_sum.cpp](../tests/library_checker/data_structure/point_add_range_sum.cpp)<br>[data_structure/point_set_range_composite.cpp](../tests/library_checker/data_structure/point_set_range_composite.cpp)<br>[data_structure/point_set_range_composite_large_array.cpp](../tests/library_checker/data_structure/point_set_range_composite_large_array.cpp)<br>[data_structure/predecessor_problem.cpp](../tests/library_checker/data_structure/predecessor_problem.cpp)<br>[data_structure/range_add_range_min.cpp](../tests/library_checker/data_structure/range_add_range_min.cpp)<br>[data_structure/range_affine_point_get.cpp](../tests/library_checker/data_structure/range_affine_point_get.cpp)<br>[data_structure/range_affine_range_sum.cpp](../tests/library_checker/data_structure/range_affine_range_sum.cpp)<br>[data_structure/segment_add_get_min.cpp](../tests/library_checker/data_structure/segment_add_get_min.cpp)<br>[data_structure/static_range_count_distinct.cpp](../tests/library_checker/data_structure/static_range_count_distinct.cpp)<br>[data_structure/static_range_frequency.cpp](../tests/library_checker/data_structure/static_range_frequency.cpp)<br>[data_structure/static_range_sum.cpp](../tests/library_checker/data_structure/static_range_sum.cpp)<br>[data_structure/static_rmq.cpp](../tests/library_checker/data_structure/static_rmq.cpp)<br>[data_structure/unionfind.cpp](../tests/library_checker/data_structure/unionfind.cpp)<br>[enumerative_combinatorics/montmort_number.cpp](../tests/library_checker/enumerative_combinatorics/montmort_number.cpp)<br>[graph/counting_spanning_trees_directed.cpp](../tests/library_checker/graph/counting_spanning_trees_directed.cpp)<br>[graph/counting_spanning_trees_undirected.cpp](../tests/library_checker/graph/counting_spanning_trees_undirected.cpp)<br>[graph/minimum_spanning_tree.cpp](../tests/library_checker/graph/minimum_spanning_tree.cpp)<br>[linear_algebra/determinant_of_matrix.cpp](../tests/library_checker/linear_algebra/determinant_of_matrix.cpp)<br>[linear_algebra/determinant_of_matrix_arbitrary_mod.cpp](../tests/library_checker/linear_algebra/determinant_of_matrix_arbitrary_mod.cpp)<br>[other/longest_increasing_subsequence.cpp](../tests/library_checker/other/longest_increasing_subsequence.cpp)<br>[string/z_algorithm.cpp](../tests/library_checker/string/z_algorithm.cpp)<br>[tree/lowest_common_ancestor.cpp](../tests/library_checker/tree/lowest_common_ancestor.cpp)<br>[tree/tree_diameter.cpp](../tests/library_checker/tree/tree_diameter.cpp) |
| [fast_io_simd.hpp](../lib/utils/fast_io_simd.hpp) | [文档](utils/fast_io_simd.md) | [片段](../sublime_snippets/utils/fast_io_simd.sublime-snippet) | [data_structure/associative_array.cpp](../tests/library_checker/data_structure/associative_array.cpp)<br>[sample/many_a_plus_b.cpp](../tests/library_checker/sample/many_a_plus_b.cpp)<br>[sample/many_a_plus_b_128bit.cpp](../tests/library_checker/sample/many_a_plus_b_128bit.cpp) |
| [timer.hpp](../lib/utils/timer.hpp) | [文档](utils/timer.md) | [片段](../sublime_snippets/utils/timer.sublime-snippet) | — |
| [y_combinator.hpp](../lib/utils/y_combinator.hpp) | [文档](utils/y_combinator.md) | [片段](../sublime_snippets/utils/y_combinator.sublime-snippet) | — |

## Library Checker 程序

| 分类 | 数量 | 路径 |
| --- | ---: | --- |
| convolution | 2 | [浏览](../tests/library_checker/convolution/) |
| data_structure | 16 | [浏览](../tests/library_checker/data_structure/) |
| enumerative_combinatorics | 1 | [浏览](../tests/library_checker/enumerative_combinatorics/) |
| graph | 3 | [浏览](../tests/library_checker/graph/) |
| linear_algebra | 2 | [浏览](../tests/library_checker/linear_algebra/) |
| other | 1 | [浏览](../tests/library_checker/other/) |
| sample | 3 | [浏览](../tests/library_checker/sample/) |
| string | 1 | [浏览](../tests/library_checker/string/) |
| tree | 2 | [浏览](../tests/library_checker/tree/) |

测试源文件首行保留对应题目链接。完整评测及性能复现见 [benchmark 说明](../benchmarks/README.md)。
