# LCA from an ordered parent array

The existing [Library Checker LCA program](../../tests/library_checker/tree/lowest_common_ancestor.cpp) combines [parent_to_dfn](../tree/parent_to_dfn.md) with a [blocked sparse table](../ds/sparse_table/blocked_sparse_table.md). Its specialized contract is important: every parent satisfies p[i] < i, and every query satisfies u < v.

These are explicit conditions in the pinned upstream [statement](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/tree/lca/task.md) and [verifier](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/tree/lca/verifier.cpp). A self query is outside this program's input domain and does not justify adding a branch to its timed query loop.

## Reduction

Let dfn[x] be a vertex's DFS preorder position. At preorder position dfn[x], store p[x] for each nonroot vertex. For distinct query vertices, let L and R be the smaller and larger preorder positions. The answer is the minimum parent label over [L + 1, R + 1).

The preorder slice traverses the boundary between the queried vertices' subtrees. It includes a parent label equal to their LCA, and every other relevant parent is the LCA or one of its descendants. Since parent labels are strictly smaller than child labels, the smallest label in this slice is the LCA. The root's unused parent slot is excluded because the left endpoint is at least 1.

Distinct vertices have distinct preorder positions, so this interval is always nonempty. The generic sparse table returns its identity for an empty interval; that behavior is correct for the container and is not an LCA answer for arbitrary self queries. Reusing this reduction in another problem requires checking that problem's own query domain first.

## Executable example

This example deliberately uses only queries satisfying the official constraint u < v, including ancestor-descendant pairs and vertices in different subtrees.

<!-- runnable-example -->
```cpp
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>
#include "lib/tree/parent_to_dfn.hpp"
#include "lib/ds/sparse_table/blocked_sparse_table.hpp"

struct Minimum {
  using Info = uint32_t;
  static Info id() { return std::numeric_limits<Info>::max(); }
  static Info op(Info a, Info b) { return std::min(a, b); }
};

int main() {
  std::vector<uint32_t> parent{0, 0, 0, 1, 1, 2};
  auto dfn = parent_to_dfn(parent);
  std::vector<uint32_t> boundary(parent.size());
  for (uint32_t i = 1; i < parent.size(); ++i) boundary[dfn[i]] = parent[i];
  BlockedSparseTable<Minimum, 64> rmq(boundary.begin(), boundary.end());
  auto lca = [&](uint32_t u, uint32_t v) {
    uint32_t left = std::min(dfn[u], dfn[v]);
    uint32_t right = std::max(dfn[u], dfn[v]);
    return rmq.prod(left + 1, right + 1);
  };
  assert(lca(0, 5) == 0);
  assert(lca(1, 4) == 1);
  assert(lca(3, 4) == 1);
  assert(lca(3, 5) == 0);
  assert(lca(2, 5) == 2);
}
```

## Scope and cost

parent_to_dfn takes O(n) time and space. The blocked table adds O(n + (n/B) log(n/B)) preprocessing and space; with the program's fixed B = 64, queries take constant time in n. The source avoids recursive DFS by relying on the ordered-parent representation.

This recipe is not a general tree input parser. A tree whose parent labels are arbitrary must first be represented in a form meeting parent_to_dfn's precondition, or handled with a different traversal. No such conversion is needed for the official problem.
