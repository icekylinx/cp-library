# Verification and benchmark interpretation

Validate a program against its actual input contract before making correctness or performance changes. Documentation examples, compilation, official checker results, and benchmark measurements provide different evidence.

## Read the contract first

For a Library Checker program, inspect the pinned problem's task.md, info.toml, and verifier.cpp. Check the valid dimensions, endpoint rules, query ordering, modulus restrictions, and operation guarantees. The verifier specifies accepted input; a fabricated case outside that domain does not demonstrate a defect in a specialized solution.

For example, the [LCA problem](../recipes/lca.md) requires u < v. Its query interval is nonempty, so adding a self-query branch is unnecessary. Likewise, whether an empty array, empty interval, missing key, or deletion of an absent element is permitted must be established separately for each problem. Do not infer one problem's guarantees from another's interface.

When investigating a suspected failure, retain the exact input, upstream revision, compilation command, process exit code, and checker message. Confirm that the input satisfies the official verifier before attributing the failure to the library. Preserve a correct implementation rather than adding speculative defensive branches to hot loops.

## Levels of evidence

| Check | What it establishes | What it does not establish |
| --- | --- | --- |
| Compile a program | The selected compiler accepts that translation unit. | Runtime correctness or support for another platform. |
| Run a documentation example | The shown model and calls behave as asserted. | Correctness of every model or boundary value. |
| Match upstream data hashes | Inputs and expected outputs match the pinned official dataset. | Correctness of the candidate program. |
| Pass the official checker on every registered case | Candidate output is accepted for that revision's registered cases. | A proof over all possible valid inputs. |
| Repeat a timed run | Observed whole-program timing under the recorded conditions. | An online rank, a universal speedup, or isolated operation latency. |

## Documentation checks

Run from the project root:

```sh
python3 tools/check_docs.py --jobs 3
```

This checks one module document per header, verifies local Markdown link destinations, and compiles and runs blocks marked with `runnable-example`. Interface sketches and fragments without that marker are explanatory. Examples use C++23 and the current project include directory.

## Official programs

The [benchmark configuration](../../benchmarks/problems.json) maps every existing Library Checker problem ID to its program. For a focused check and measurement, choose problem IDs explicitly:

```sh
python3 tools/benchmark.py --generate --problems lca segment_add_get_min --output benchmarks/results/focused-run.json
```

The tool runs each program on every official registered input and invokes the official output checker. It checks output structure and semantic validity, rather than demanding byte equality; this matters for problems such as spanning trees or tree diameter, where several valid outputs may exist.

Strict mode stops on the first failure. Add --keep-going to retain runtime failures, timeouts, or checker rejections and continue evaluating the remaining registered cases and programs. Failed cases are checked with the pinned official input verifier; a zero verifier exit confirms that it accepts the input. A failing program is not timed. A completed report with failures exits nonzero and records completed_with_failures, rather than claiming success. Generation, data-hash, or compilation errors remain fatal.

Intermediate outputs and compiled input verifiers remain in build/benchmark for diagnosis. Preserve the source and record the outcome when a repair is outside the requested scope.

## Timing policy

The current default selects up to three timed inputs per problem. It prefers one large input from each generator family, identified by removing the trailing seed number from the filename, then fills any remaining slots by input size. Equal byte sizes are ordered by filename. Inputs matching no_output* are excluded from timing, but still checked for correctness. Use --case-policy largest to select by size alone; --exclude-case adds another timing exclusion glob. Use --include-no-output when deliberately reproducing a policy that timed no-output workloads.

Every measured program uses a regular stdin file and /dev/null for stdout, is pinned to the recorded logical CPU, and receives the same number of warmups. Candidate/reference order alternates between measured rounds. Compiler command-line flags match, while source-level optimize/target directives remain active and are recorded.

A table cell reports median [minimum, maximum] wall-clock milliseconds. The range describes the observed runs, not a statistical confidence interval. CPU time and every individual sample remain in the JSON. Small inputs can be dominated by process startup; no lower-bound subtraction is applied.

## Comparing records

Use the [coverage index](../../benchmarks/coverage.md) to locate each problem's report. Compare the input hash, candidate source hash, upstream commit, library source digest, compiler flags, CPU, case policy, and warmup/repetition counts before comparing timings across reports.

The original eight-problem record uses two largest inputs per problem and includes a no-output workload. The expansion uses three inputs with generator diversity and excludes no-output workloads from timing. These differences are intentional and are recorded rather than treated as regressions.

A module directly or transitively included by a passing program is exercised by that program; this does not imply exhaustive API coverage. Modules without an official-program consumer still rely on documentation examples or other explicit tests. Keep that distinction when assessing verification coverage.
