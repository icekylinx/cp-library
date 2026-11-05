# Library Checker benchmark

Check library programs with official generators and checkers, then compare local runtime against `sol/correct.cpp` from the same upstream revision. Default problems are listed in [problems.json](problems.json).

## Recorded measurements

- [November 5, 2026 results](results/2026-11-05.md): all 158 official cases across eight problems passed verification. Sixteen large inputs were timed with one warmup and five measured runs per program.
- [Raw data](results/2026-11-05.json): individual timings, compiler commands, source and input hashes, and checker records.
- This run uses upstream commit [`73876f1cbcf082d2b22722924b924d3d65a2f00e`](https://github.com/yosupo06/library-checker-problems/tree/73876f1cbcf082d2b22722924b924d3d65a2f00e), fetched from the official default branch on November 5, 2026.

## Download or update generators

The official problem repository includes each problem's `gen/` directory, checker, reference solutions, and registered case hashes. Keep it under the Git-ignored `local/` directory so generators and large datasets remain local.

```sh
mkdir -p local
git clone --depth 1 https://github.com/yosupo06/library-checker-problems.git local/library-checker-problems
```

Update an existing checkout with:

```sh
git -C local/library-checker-problems pull --ff-only
git -C local/library-checker-problems rev-parse HEAD
```

To reproduce an earlier report, use the upstream commit recorded in that report. Regenerate data after updating upstream to create a new measurement version. The official generator bundles its TOML parser; `colorlog` is an optional dependency for colored logs.

## Generate, verify, and measure

Requirements are Linux, Python 3.11 or later, and GCC with C++23 support. Some library implementations require AVX2, which the machine used for the recorded run supports. Run all commands from the project root.

```sh
python3 tools/benchmark.py --generate
```

The default run covers eight problems. It generates every official case, verifies the SHA256 hashes of inputs and expected outputs, runs the library program, and invokes the official checker for every case. Any failure stops the run without writing a successful report. Both programs are recompiled before timing. The two largest inputs by byte size are selected per problem, with filename order breaking ties.

Skip generation when the data already exist:

```sh
python3 tools/benchmark.py --problems unionfind staticrmq --output benchmarks/results/local-comparison.json
```

The process is pinned to the lowest available logical CPU by default; select another with `--cpu`. Defaults are one warmup and five measured runs. Adjust these with `--warmups`, `--repetitions`, and `--cases`. Each program has a 30-second per-case timeout, configurable with `--timeout`. Default compiler flags are `-std=c++23 -O2`; override them with an option such as `--flags='-std=c++23 -O3'`.

Reports are written to `benchmarks/results/YYYY-MM-DD.json` and a matching Markdown file, using the UTC date. Existing reports are never overwritten; choose a new `--output` for another run on the same day. Intermediate executables, checker outputs, and generation logs are stored in `build/benchmark/`.

The upstream tree-diameter reference solution is recursive and needs a large stack for long paths. This tool sets the child-process stack limit to unlimited. When invoking the official generator directly, first run `ulimit -s unlimited`, as described in the [upstream instructions](https://github.com/yosupo06/library-checker-problems#how-to-use).

## Interpreting results

Measurements cover complete-program wall-clock time, including startup, input parsing, computation, and output formatting. Input comes from regular files to support the library's default mmap path, and output goes to `/dev/null`. Checker execution is excluded from timing. Both programs use the same warmup and output procedure, and execution order alternates between rounds. Raw data also include CPU time; summary tables use median wall-clock time.

Equal command-line flags do not remove differences caused by source-level GCC optimize/target directives; those directives are recorded in the JSON. The CPU is shared, and system load and dynamic frequency can affect results. These measurements cover more than the algorithm kernel and cannot be converted into current Library Checker online rankings. A `no_output` case primarily measures its workload without queries.
