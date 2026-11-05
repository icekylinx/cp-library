# Local Library Checker benchmark

Run started: 2026-11-05T13:21:12.454918+00:00.

Upstream: [yosupo06/library-checker-problems](https://github.com/yosupo06/library-checker-problems), commit [`73876f1cbcf0`](https://github.com/yosupo06/library-checker-problems/tree/73876f1cbcf082d2b22722924b924d3d65a2f00e).

## Methodology

- CPU: Intel(R) Core(TM) i5-9500 CPU @ 3.00GHz; pinned to logical CPU 0.
- Platform: `Linux-7.2.14-arch1-1-x86_64-with-glibc2.44`.
- Compiler: `g++ (GCC) 16.2.1 20260810`; both programs use the command-line flags `-std=c++23 -O2`.
- Source-level GCC optimize/target directives are retained. Equal command-line flags do not imply equal effective optimization settings; per-problem directives are recorded in the JSON.
- Generate every official case, verify inputs and expected outputs against upstream SHA256 hashes, and check the library program on every case with the official checker.
- For each problem, prefer the largest input from each generator family, then fill remaining slots by size, up to 3 cases. Break size ties by filename.
- Exclude these filename patterns from timing only: `no_output*`. Every official case still participates in correctness checks.
- Warm up each program 1 time(s) per case, then measure 5 runs; alternate which program runs first.
- Read stdin from regular files and send stdout to `/dev/null`. The table reports median wall-clock time, including process startup, input parsing, and output formatting; compilation, generation, and checking are excluded.
- File caches are warm, the CPU is shared, and frequency may vary. These results compare complete programs on this machine, not current online rankings or isolated data-structure operations.

## Results

The ratio is reference time divided by library time; values above 1 mean the library was faster on that case. Programs with any verification failure are not timed.

Each timing cell is the median [minimum, maximum] in milliseconds. The range describes observed run-to-run spread, not a confidence interval.

| Problem | Correctness | Timed case | Input MiB | Library ms | Reference ms | Ratio |
| --- | --- | --- | ---: | ---: | ---: | ---: |
| [static_range_count_distinct](https://judge.yosupo.jp/problem/static_range_count_distinct) | 11/11 | `random_03.in` | 11.23 | 46.135 [42.248, 51.291] | 333.004 [322.046, 350.468] | 7.22x |
| [static_range_count_distinct](https://judge.yosupo.jp/problem/static_range_count_distinct) | 11/11 | `small_q_00.in` | 4.72 | 2.415 [2.331, 5.213] | 143.015 [139.061, 150.667] | 59.21x |
| [static_range_count_distinct](https://judge.yosupo.jp/problem/static_range_count_distinct) | 11/11 | `small_n_02.in` | 1.91 | 12.590 [12.104, 13.721] | 74.973 [74.725, 76.045] | 5.96x |
| [static_range_frequency](https://judge.yosupo.jp/problem/static_range_frequency) | 11/11 | `random_03.in` | 16.00 | 55.185 [47.940, 60.985] | 308.810 [296.314, 322.195] | 5.60x |
| [static_range_frequency](https://judge.yosupo.jp/problem/static_range_frequency) | 11/11 | `small_n_01.in` | 6.67 | 18.906 [17.393, 20.497] | 140.935 [126.932, 144.169] | 7.45x |
| [static_range_frequency](https://judge.yosupo.jp/problem/static_range_frequency) | 11/11 | `small_q_00.in` | 4.72 | 2.493 [2.259, 2.680] | 73.687 [73.461, 75.777] | 29.56x |

22/22 official cases passed the checker across 2 problems; 0 program(s) had verification failures.

## Reproduction and raw data

See the [benchmark guide](../README.md) for commands. The matching JSON file contains every wall-clock and CPU-time sample, compilation commands, input and source hashes, and case verification records.

Library source digest for this run: `ada135d84dc3504636f131e332a7ee360abe163ae47e803fd2c76884c9964428`.
