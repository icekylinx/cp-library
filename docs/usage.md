# Build and I/O guide

Build contest programs from the project root with the project directory on the include path. The examples below use GCC with C++23; some test programs require C++23 features even though many headers work with C++20.

## Compile and run

```sh
mkdir -p build
g++ -std=c++23 -O2 -I. tests/library_checker/data_structure/unionfind.cpp -o build/unionfind
printf '4 5\n0 0 1\n1 0 1\n1 0 2\n0 1 2\n1 0 2\n' > build/unionfind.in
./build/unionfind < build/unionfind.in
```

The expected output is `1`, `0`, and `1`, on separate lines. To start a new program:

```sh
mkdir -p local
cp template.cpp local/main.cpp
g++ -std=c++23 -O2 -I. local/main.cpp -o build/main
```

The template creates FastIO and calls solve_main once. Adjust its input protocol to the problem; the template does not infer a test-case count.

## Input modes and debug flags

| Setting | Effect | Usage condition |
| --- | --- | --- |
| Default Unix FastIO | Memory-map stdin as a regular file. | Use a nonempty file redirected to stdin. |
| `-DDISABLE_MMAP` | Use buffered fread input. | Required for pipes or terminal input with FastIO. |
| `-DLOCAL` | Enable CHECK assertions; existing test mains also use test.in and test.out. | Place those files in the process working directory. |
| `-DNDEBUG` | Disable standard assert expressions. | Do not combine casually with mains that put freopen inside assert. |

For piped input, compile a separate buffered-input executable:

```sh
g++ -std=c++23 -O2 -DDISABLE_MMAP -I. tests/library_checker/data_structure/unionfind.cpp -o build/unionfind-buffered
printf '4 2\n0 0 1\n1 0 1\n' | ./build/unionfind-buffered
```

The output is one line containing `1`. Do not compare this input mode with a different mode without recording the change in a benchmark.

FastIO requires valid, complete input and does not provide a general EOF/error protocol. Prefer `io >> value` unless the caller intentionally satisfies the cursor and delimiter contract of read or read_small. Reaching EOF is not a valid reason to call skip_space when no further token is required.

## Scalar and SIMD headers

Choose either [fast_io.hpp](utils/fast_io.md) or [fast_io_simd.hpp](utils/fast_io_simd.md) in a translation unit. They define the same type names and cannot be included together. The SIMD version requires a compatible x86 CPU with AVX2 and supports GNU 128-bit integer I/O. The fixed-modulus determinant header also enables AVX2.

These headers are primarily intended for single-translation-unit contest programs. In particular, the header-level endl definition is not a general multi-file linking interface. FastIO owns pointers: initialize each instance once, keep its FILE objects alive until destruction, and do not copy it.

## Expand a submission

```sh
python3 expander.py tests/library_checker/data_structure/unionfind.cpp -o build/unionfind.cpp
g++ -std=c++23 -O2 build/unionfind.cpp -o build/unionfind-expanded
```

The expander replaces quoted includes resolving inside lib, removes pragma once lines, deduplicates angle-bracket include directives, and normalizes whitespace. Quoted includes outside lib remain includes. Run `python3 expander.py --help` for options.

Its default root is the current directory. To invoke it elsewhere, use --root with the project path and a source path relative to that root. The output file path is resolved from the caller's current directory. Keep the same compiler macros when comparing an expanded program against its original.

## Contracts and validation

Sizes and indices normally use uint32_t, intervals are half-open, and model operations have the laws documented on each module page. Specialized problem programs may deliberately rely on stronger official input guarantees. Read those guarantees before proposing a special case.

Follow the [documentation guide](README.md) for module selection, the [verification guide](guides/verification.md) for checks, and the [benchmark guide](../benchmarks/README.md) for reproducible measurements. The project is licensed under [MIT](../LICENSE).
