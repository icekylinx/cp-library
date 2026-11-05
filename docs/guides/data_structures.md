# Choosing a data structure

Start with the operations required by the problem, then check whether the value type satisfies the structure's algebraic contract. A faster implementation of the wrong contract will still produce incorrect answers.

## Range structures

The costs below assume constant-time model operations. n is the number of elements; B is the block size, normally 16.

| Structure | Updates | Queries | Build | Query cost | Model requirement |
| --- | --- | --- | --- | --- | --- |
| [FenwickTree](../ds/fenwick_tree/fenwick_tree.md) | Point increment, O(log n) | Prefix or interval aggregate | O(n) | O(log n) | Commutative group for the full interface |
| [FastFenwickTree](../ds/fenwick_tree/fast_fenwick_tree.md) | Point increment, O(log n) | Prefix or interval aggregate | O(n) | O(log n) | Same as FenwickTree; altered storage layout |
| [FenwickTreeRange](../ds/fenwick_tree/fenwick_tree_range.md) | Interval increment, O(log n) | Prefix or interval aggregate | O(n) | O(log n) | Commutative group plus constant-time repetition |
| [SegTree](../ds/seg_tree/seg_tree.md) | Point assignment, O(log n) | Ordered interval aggregate or fold | O(n) | O(log n) | Monoid; commutativity is unnecessary |
| [LazySegTree](../ds/seg_tree/lazy_seg_tree.md) | Interval action or point assignment, O(log n) | Interval aggregate | O(n) | O(log n) | Monoid with composable compatible actions |
| [TagSegTree](../ds/seg_tree/tag_seg_tree.md) | Interval action or point assignment, O(log n) | Point value | O(n) | O(log n) | Values with composable actions; no aggregate operation |
| [SparseTable](../ds/sparse_table/sparse_table.md) | Rebuild only | Static interval aggregate | O(n log n) | O(1) | Associative, idempotent operation with an identity |
| [BlockedSparseTable](../ds/sparse_table/blocked_sparse_table.md) | Rebuild only | Static interval aggregate | O(n + (n/B) log(n/B)) | O(B) within a block, O(1) across blocks | Same idempotence requirement |

Use point assignment for function replacement or arbitrary value replacement. Fenwick add is an increment: for integer sums, assigning a new value requires retaining the old value and adding the difference. Ordinary range sums do not satisfy a sparse table's idempotence contract.

If updates affect ranges but every query asks about one point, TagSegTree avoids storing internal aggregates. If both updates and queries affect ranges, LazySegTree needs enough information to transform an entire aggregate, such as storing both sum and segment length.

## Other choices

| Requirement | Module | Important condition |
| --- | --- | --- |
| Merge components and test connectivity | [DSU](../ds/dsu/dsu.md) | No deletion, splitting, or rollback |
| Predecessor or successor in a bounded integer universe | [IntegerSet](../ds/set/integer_set.md) | Queries are strict; parameters must remain below n |
| Insert whole lines on an integer coordinate domain | [LiChaoTree](../ds/li_chao_tree/li_chao_tree.md) | Safe finite domain and sentinel line |
| Whole-line insertion at known query coordinates | [LiChaoTreeOffline](../ds/li_chao_tree/li_chao_tree_offline.md) | Sort query coordinates before building |
| Segment-restricted line insertion | [LiChaoSegTreeOffline](../ds/li_chao_tree/li_chao_seg_tree_offline.md) | Convert original intervals to compressed index intervals |
| Fixed-capacity integer-key lookup | [HashMap](../ds/hash_map/hash_map.md) or [SafeHashMap](../ds/hash_map/safe_hash_map.md) | Reserve empty slots; no automatic growth or individual deletion |

## Memory planning

A segment tree rounds the leaf count up to a power of two and stores roughly twice that count in its aggregate array. LazySegTree also stores tags and occupancy flags. FenwickTree stores n + 1 values; FastFenwickTree adds a gap after each 1024 logical positions. FenwickTreeRange uses two arrays.

A fixed-capacity hash map allocates its full key and value arrays inside the object, plus an occupancy bitmap. Its main storage is approximately `2^N * (sizeof(K) + sizeof(V))` bytes, excluding padding and auxiliary storage. Allocate large tables statically or on the heap to avoid stack exhaustion. At least one slot must remain empty; a lower load factor generally reduces probing, but the best choice depends on the workload.

IntegerSet allocates according to 2^B even when its constructor receives a much smaller n. Choose B from the required universe rather than relying on the default 24.

## Boundary checklist

- Preserve the distinction between n, a valid element index, and an interval endpoint. An endpoint may equal n; an element index may not.
- Verify that the identity and all intermediate arithmetic fit the selected integer types.
- For Fenwick initial-value builders, Info{} must equal id() in the current implementation.
- Check which boundary cases the problem actually permits before changing a solution. The Library Checker LCA problem requires u < v, so its solution needs no self-query branch.
- Read the [algebra guide](algebra.md) before supplying a noncommutative operation or tag.
