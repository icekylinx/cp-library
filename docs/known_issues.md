# Recorded verification failures

The [November 5, 2026 expanded evaluation](../benchmarks/results/2026-11-05-expanded.md) records two program failures on pinned official input. The matching [raw report](../benchmarks/results/2026-11-05-expanded.json) contains source hashes, input hashes, process exit codes, input-verifier results, and compilation commands. No repair is included in this record.

## Zero-query official cases

| Program | Official case | Input header | Candidate result | Input verifier | Expected output |
| --- | --- | --- | --- | --- | --- |
| [Static range count distinct](../tests/library_checker/data_structure/static_range_count_distinct.cpp) | small_q_00.in | N = 500000, Q = 0 | SIGSEGV, exit -11 | accepted, exit 0 | empty |
| [Static range frequency](../tests/library_checker/data_structure/static_range_frequency.cpp) | small_q_00.in | N = 500000, Q = 0 | SIGSEGV, exit -11 | accepted, exit 0 | empty |

Both official files have SHA256 `1796a10a10f7a4352d8efc5315d9d720320fb9ee6911bedb8dd36e047b64fdd0`. Their respective pinned [count-distinct verifier](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/data_structure/static_range_count_distinct/verifier.cpp) and [frequency verifier](https://github.com/yosupo06/library-checker-problems/blob/73876f1cbcf082d2b22722924b924d3d65a2f00e/data_structure/static_range_frequency/verifier.cpp) permit Q = 0. Each program passed its other ten registered cases in this evaluation.

These programs receive no timing results in the expansion. A failing complete program also does not qualify as a passing consumer in the [header coverage index](../benchmarks/coverage.md).

## Reproduction

After obtaining the pinned upstream data as described in the [benchmark guide](../benchmarks/README.md), run from the project root:

```sh
mkdir -p build
g++ -std=c++23 -O2 -I. tests/library_checker/data_structure/static_range_count_distinct.cpp -o build/distinct
./build/distinct < local/library-checker-problems/data_structure/static_range_count_distinct/in/small_q_00.in
```

The recorded run terminated with SIGSEGV. To verify that this official input satisfies the problem contract:

```sh
g++ -std=c++23 -O2 -I local/library-checker-problems/common local/library-checker-problems/data_structure/static_range_count_distinct/verifier.cpp -o build/distinct-input-verifier
./build/distinct-input-verifier < local/library-checker-problems/data_structure/static_range_count_distinct/in/small_q_00.in
```

The input verifier exits zero. Use the same steps with static_range_frequency to reproduce its record, or run the evaluation tool with --keep-going and a new output path to collect checker results and failure metadata without modifying either source.
