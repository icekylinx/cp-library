#!/usr/bin/env python3
"""Check header documentation, local links and runnable C++ examples."""

import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--cxx", default="g++")
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    for header in (ROOT / "lib").rglob("*.hpp"):
        doc = ROOT / "docs" / header.relative_to(ROOT / "lib").with_suffix(".md")
        if not doc.is_file():
            raise RuntimeError(f"missing documentation: {doc}")
    documents = [ROOT / "README.md", *(ROOT / "docs").rglob("*.md"), *(ROOT / "benchmarks").rglob("*.md")]
    examples = []
    links = 0
    for doc in documents:
        content = doc.read_text()
        prose = re.sub(r"```.*?```", "", content, flags=re.DOTALL)
        prose = re.sub(r"`[^`\n]*`", "", prose)
        for link in re.findall(r"\]\(([^)]+)\)", prose):
            if re.match(r"^[a-zA-Z][a-zA-Z0-9+.-]*:", link) or link.startswith("#"):
                continue
            target = link.split("#")[0]
            if not (doc.parent / target).exists():
                raise RuntimeError(f"{doc.relative_to(ROOT)}: broken local link {link}")
            links += 1
        for index, code in enumerate(re.findall(r"<!-- runnable-example -->\s*```cpp\n(.*?)```", content, re.DOTALL)):
            examples.append((doc, index, code))
    build = ROOT / "build/doc-examples"
    build.mkdir(parents=True, exist_ok=True)

    def check(example):
        doc, index, code = example
        name = doc.relative_to(ROOT).with_suffix("").as_posix().replace("/", "__") + f"__{index}"
        source, binary = build / (name + ".cpp"), build / name
        source.write_text(code, encoding="utf-8")
        compiled = subprocess.run([args.cxx, "-std=c++23", "-O2", "-I", str(ROOT), str(source), "-o", str(binary)],
                                  capture_output=True, text=True)
        if compiled.returncode:
            return f"{doc.relative_to(ROOT)}: compilation failed\n{compiled.stderr}"
        try:
            ran = subprocess.run([str(binary)], capture_output=True, timeout=10)
        except subprocess.TimeoutExpired:
            return f"{doc.relative_to(ROOT)}: example timed out"
        if ran.returncode:
            return f"{doc.relative_to(ROOT)}: example exited {ran.returncode}\n{ran.stderr.decode(errors='replace')}"
        return None

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        failures = [failure for failure in pool.map(check, examples) if failure]
    for failure in failures:
        print(failure)
    print(f"Documentation: {len(examples) - len(failures)}/{len(examples)} runnable examples; {links} valid local links")
    if failures:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
