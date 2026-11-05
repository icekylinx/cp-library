# Library Checker benchmark

Check library programs with official generators and checkers, then compare local runtime against `sol/correct.cpp` from the same upstream revision. All 31 existing Library Checker programs are configured in [problems.json](problems.json). The [coverage index](coverage.md) distinguishes passing records, recorded failures, and unmeasured programs.

## Recorded measurements

- [Zero-query fix recheck](results/2026-11-05-zero-query-fix.md) and [raw data](results/2026-11-05-zero-query-fix.json): both repaired programs pass all 22 official cases and add six timed inputs. The current-source index now has 31 passing programs, 583/583 accepted cases, and 85 timed inputs.
- [Expanded evaluation](results/2026-11-05-expanded.md) and [raw data](results/2026-11-05-expanded.json): 23 additional programs and 425 official cases. Twenty-one programs passed all cases and contributed 63 timed inputs; the two remaining programs each failed one valid zero-query case at that revision. Overall, 423/425 new cases passed. The failures are resolved by the targeted recheck above; the original [failure evidence](../docs/known_issues.md) is retained.
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
python3 tools/benchmark.py --generate --keep-going --output benchmarks/results/all-programs.json
```

With no --problems option, the run covers all 31 configured programs. It generates every official case, checks input and expected-output SHA256 hashes, runs the library program, and invokes the official output checker. Both programs are recompiled before timing.

Strict mode stops at the first verification failure. With --keep-going, runtime failures, timeouts, and checker rejections are recorded while the remaining registered cases and programs continue. Each failed case is additionally passed to the pinned input verifier, whose exit code is recorded. A program with any failed case receives no timing results. A completed evaluation with failures still exits nonzero, with status completed_with_failures in its JSON; it must not be presented as an all-passing benchmark. Generation, data-hash, and compilation errors still stop the run.

The default timing policy prefers the largest input from each generator family, then fills remaining slots by size, up to three inputs per passing program. A family is identified by removing the trailing numeric seed from its filename. Size ties use filename order. Inputs matching no_output* are omitted from timing, but remain in correctness checks. Use --case-policy largest for size-only selection, --exclude-case to add an exclusion glob, and --include-no-output to restore eligibility of no-output workloads.

Skip generation when the data already exist:

```sh
python3 tools/benchmark.py --problems unionfind staticrmq --output benchmarks/results/local-comparison.json
```

The process is pinned to the lowest available logical CPU by default; select another with `--cpu`. Defaults are one warmup, five measured runs, and three timed inputs per passing program. Adjust these with `--warmups`, `--repetitions`, and `--cases`. Each program has a 30-second per-case timeout, configurable with `--timeout`. Default compiler flags are `-std=c++23 -O2`; override them with an option such as `--flags='-std=c++23 -O3'`.

Reports are written to `benchmarks/results/YYYY-MM-DD.json` and a matching Markdown file, using the UTC date. Existing reports are never overwritten; choose a new `--output` for another run on the same day. Intermediate executables, checker outputs, and generation logs are stored in `build/benchmark/`.

The upstream tree-diameter reference solution is recursive and needs a large stack for long paths. This tool sets the child-process stack limit to unlimited. When invoking the official generator directly, first run `ulimit -s unlimited`, as described in the [upstream instructions](https://github.com/yosupo06/library-checker-problems#how-to-use).

## Interpreting results

Measurements cover complete-program wall-clock time, including startup, input parsing, computation, and output formatting. Input comes from regular files to support the library's default mmap path, and output goes to `/dev/null`. Checker execution is excluded from timing. Both programs use the same warmup and output procedure, and execution order alternates between rounds. Raw data also include CPU time. Summary cells show median [minimum, maximum] wall-clock milliseconds; this observed range is not a confidence interval. The runner SHA256 and pinned task.md, verifier.cpp, and info.toml hashes are recorded for the expansion.

Equal command-line flags do not remove differences caused by source-level GCC optimize/target directives; those directives are recorded in the JSON. The CPU is shared, and system load and dynamic frequency can affect results. These measurements cover more than the algorithm kernel and cannot be converted into current Library Checker online rankings. A `no_output` case primarily measures its workload without queries.

## Reproducing the original selection policy

The original eight-problem report retains its historical policy of two largest inputs, including no-output cases. Use these options with that report's problem IDs to reproduce its selection rather than the newer diversity policy:

```sh
python3 tools/benchmark.py --problems unionfind staticrmq point_add_range_sum range_affine_range_sum line_add_get_min zalgorithm bitwise_xor_convolution tree_diameter --cases 2 --case-policy largest --include-no-output --output benchmarks/results/original-policy-repeat.json
```

This makes a new record; it does not overwrite the original measurements. Keep the same upstream revision and inspect input hashes before making a timing comparison.

## Refreshing coverage

```sh
python3 tools/update_benchmark_index.py
```

The index selects the newest completed evaluation per program, checks its program and library hashes against the current sources, and lists direct or transitive header consumers only for all-passing programs. Recorded failures are visible, not silently replaced with an earlier passing result. See the [verification guide](../docs/guides/verification.md) before treating a failure on a fabricated input as evidence of a defect.
