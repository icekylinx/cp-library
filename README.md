# cp-library

A C++ competitive programming library with 38 headers, 38 module documents, 31 Library Checker programs, and 37 Sublime Text snippets. Licensed under MIT.

## Layout and documentation

| Path | Contents |
| --- | --- |
| [lib/](lib/) | Algorithms, data structures, mathematics, strings, trees, and utilities |
| [tests/](tests/) | Library Checker programs |
| [sublime_snippets/](sublime_snippets/) | Sublime Text snippets organized by module |
| [docs/README.md](docs/README.md) | Build instructions, selection guides, algebra contracts, and recipes |
| [docs/catalog.md](docs/catalog.md) | Index of module documents, headers, snippets, and examples |
| [docs/usage.md](docs/usage.md) | Usage reference |
| [benchmarks/README.md](benchmarks/README.md) | Official generators, local verification, and benchmark reproduction |
| [benchmarks/coverage.md](benchmarks/coverage.md) | Per-program benchmark records and current-source header consumers |
| [docs/benchmarks.md](docs/benchmarks.md) | Historical performance records and submission links from the original README |
| [template.cpp](template.cpp) | Contest program template |
| [expander.py](expander.py) | Expand includes under `lib/` into a single source file |
| `local/` | Local generators, scratch programs, and archives; ignored by Git |
| `build/` | Compiled programs and expanded source files; ignored by Git |

## Quick start

Run the following commands from the project root. Library headers require C++20 or later; `-std=c++23 -O2` with GCC is the usual configuration. Some implementations use GNU extensions or platform-specific instructions.

Compile and run the existing union-find example:

```sh
mkdir -p build
g++ -std=c++23 -O2 -I. tests/library_checker/data_structure/unionfind.cpp -o build/unionfind
printf '4 5\n0 0 1\n1 0 1\n1 0 2\n0 1 2\n1 0 2\n' > build/unionfind.in
./build/unionfind < build/unionfind.in
```

The expected output is three lines: `1`, `0`, and `1`.

On Unix, FastIO uses `mmap` with regular input files by default, which is why the example uses file redirection. For pipes or terminal input, compile with `-DDISABLE_MMAP` to enable buffered `fread` input.

Start a new program by copying `template.cpp` into `local/`. Include headers through paths such as `lib/utils/fast_io.hpp`, with the project root on the include path:

```sh
mkdir -p local
cp template.cpp local/main.cpp
g++ -std=c++23 -O2 -I. local/main.cpp -o build/main
```

When `LOCAL` is defined, the template and existing test programs redirect input and output to `test.in` and `test.out` in the current working directory. Leave `-DLOCAL` out when using standard input and output.

## Expand into a single file

```sh
python3 expander.py tests/library_checker/data_structure/unionfind.cpp -o build/unionfind.cpp
g++ -std=c++23 -O2 build/unionfind.cpp -o build/unionfind-expanded
```

`expander.py` resolves project paths from the current directory by default. Use `--root` to specify the project root when calling it from another directory. See `python3 expander.py --help` for additional options.

## Conventions

- Data structures use zero-based indices and half-open intervals `[left, right)`.
- Sizes are generally represented by `uint32_t`; check individual implementations before using zero-sized structures.
- The library performs limited argument checking. Callers must satisfy each implementation's preconditions.
- Sublime Text snippets and headers are maintained separately; check the version you need before use.

## Documentation and validation

Each header has a corresponding `docs/module/path/name.md` page covering requirements, public interfaces, complexity, and examples. `online_inverse.hpp` remains an unimplemented placeholder, as documented on its page.

```sh
python3 tools/update_catalog.py
python3 tools/update_benchmark_index.py
python3 tools/check_docs.py
```

These commands rebuild the module catalog and benchmark coverage index, then check header-document coverage and local links and compile and run standalone examples. See the [benchmark guide](benchmarks/README.md) for official data generation, checker verification, and timing.

## Local benchmark

The [coverage index](benchmarks/coverage.md) now includes evaluations for all 31 existing programs. With the [zero-query fix recheck](benchmarks/results/2026-11-05-zero-query-fix.md), all 31 programs pass every registered case: 583/583 official cases, with 85 timed inputs in total.

The two [previously recorded zero-query failures](docs/known_issues.md) are resolved with one initialization-time return in each affected program. Their complete 22-case recheck passed. Query loops and shared library headers remain unchanged.

The original eight-problem measurements are preserved. The expansion adds 23 evaluations, generator-diverse timing inputs, observed timing ranges, and input-contract hashes. Raw JSON records retain individual samples and source/input provenance. Older online performance data remain in the [historical records](docs/benchmarks.md).

## License

[MIT License](LICENSE)
