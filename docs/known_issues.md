# Resolved verification failures

The zero-query failures below are resolved in the current sources. The [targeted recheck](../benchmarks/results/2026-11-05-zero-query-fix.md) and [raw data](../benchmarks/results/2026-11-05-zero-query-fix.json) confirm that both programs now pass all 22 registered official cases, including the exact input that previously crashed. They also contribute six timed inputs to the [current-source coverage index](../benchmarks/coverage.md).

## Cause and fix

Both programs previously called skip_space after reading the array even when Q = 0. No query token remained, so whitespace scanning continued beyond the end of the input. Each solve_main now returns immediately after reading N and Q when Q is zero. This also avoids unnecessary array processing and offline preprocessing for a computation with no answers.

The check occurs once during initialization. The query loops, data structures, and shared FastIO implementation are unchanged. Positive-query behavior was checked against every official case for both programs.

Additional verifier-accepted fixtures cover N = Q = 0, a nonempty array with Q = 0, an empty array with queries, duplicates, and empty intervals. Eight fixtures passed in both mmap-file and DISABLE_MMAP buffered-pipe modes, for 16 successful executions.

## Historical failure evidence

The [November 5, 2026 expanded evaluation](../benchmarks/results/2026-11-05-expanded.md) and its [raw report](../benchmarks/results/2026-11-05-expanded.json) preserve the pre-fix source hashes, input hashes, process exits, verifier results, and compilation commands.

| Program | Official case | Input header | Before the fix | After the fix | Expected output |
| --- | --- | --- | --- | --- | --- |
| [Static range count distinct](../tests/library_checker/data_structure/static_range_count_distinct.cpp) | small_q_00.in | N = 500000, Q = 0 | SIGSEGV, exit -11 | checker accepted, exit 0 | empty |
| [Static range frequency](../tests/library_checker/data_structure/static_range_frequency.cpp) | small_q_00.in | N = 500000, Q = 0 | SIGSEGV, exit -11 | checker accepted, exit 0 | empty |

Both official files have SHA256 `1796a10a10f7a4352d8efc5315d9d720320fb9ee6911bedb8dd36e047b64fdd0`. Their pinned [count-distinct verifier](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/data_structure/static_range_count_distinct/verifier.cpp) and [frequency verifier](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/data_structure/static_range_frequency/verifier.cpp) accept Q = 0. The historical failed programs were not timed; their reports remain unchanged.

## Recheck the fix

After obtaining the pinned upstream data as described in the [benchmark guide](../benchmarks/README.md), run from the project root:

```sh
mkdir -p build
g++ -std=c++23 -O2 -I. tests/library_checker/data_structure/static_range_count_distinct.cpp -o build/distinct
./build/distinct < local/library-checker-problems/data_structure/static_range_count_distinct/in/small_q_00.in
```

The program now exits zero with empty output. Use the same steps for static_range_frequency. For complete official checker verification and a fresh benchmark record:

```sh
python3 tools/benchmark.py --problems static_range_count_distinct static_range_frequency --output benchmarks/results/zero-query-recheck.json
```

The pre-fix implementations are retained in commit [91d0c3e](https://github.com/icekylinx/cp-library/commit/91d0c3ea21c40e706ee34214ee570995d2da0624) and identified by source hashes in the historical report.
