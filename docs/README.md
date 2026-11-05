# Documentation guide

Use this guide to choose a module, understand its algebraic contract, and verify a complete program. The [catalog](catalog.md) links every header to its API reference, editor snippet, and direct-include examples.

## Getting started

1. Read the [build and I/O guide](usage.md), especially the regular-file requirement of mmap input.
2. Choose a structure with the [data structure guide](guides/data_structures.md).
3. Define the required operations using the [algebra and tag composition guide](guides/algebra.md).
4. Check the individual module reference for index, capacity, and arithmetic limits.
5. Run a [verification workflow](guides/verification.md) before interpreting benchmark results.

## Guides and recipes

| Document | Purpose |
| --- | --- |
| [Data structure selection](guides/data_structures.md) | Compare operations, algebra requirements, complexity, and memory tradeoffs. |
| [Algebra and tag composition](guides/algebra.md) | Define identities, inverses, idempotent operations, and chronological tag composition. |
| [Offline coordinates](guides/offline_coordinates.md) | Prepare Li Chao queries, translate intervals, and choose safe coordinate sentinels. |
| [Verification and benchmark interpretation](guides/verification.md) | Separate compilation, executable examples, official checker results, and timing records. |
| [LCA from a parent array](recipes/lca.md) | Combine parent_to_dfn with static RMQ under the official query constraints. |
| [Module catalog](catalog.md) | Browse all 38 header references. |
| [Resolved verification failures](known_issues.md) | Historical failure evidence, the targeted fix, and recheck commands. |
| [Benchmark coverage](../benchmarks/coverage.md) | Find measured programs, official case counts, and reports. |

## API conventions

Indices are zero-based and intervals are half-open unless a page explicitly states otherwise. Dimensions normally use uint32_t. Treat model operations as constant-time when interpreting the stated data-structure complexities.

A type's public data members are not a promise that arbitrary mutations preserve its invariants. Prefer documented constructors and operations. Low-level access in specialized contest programs assumes the input format and bounds of that problem.

Examples marked with `runnable-example` can be checked by `python3 tools/check_docs.py`. That command validates the code shown, not all possible models, boundary values, or combinations of headers. In particular, include either scalar FastIO or SIMD FastIO in one translation unit, not both.

`online_inverse.hpp` is an unimplemented placeholder. Editor snippets are maintained separately from headers; code in a snippet may lag behind the current API.
