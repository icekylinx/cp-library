#!/usr/bin/env python3
"""Rebuild the header/document/snippet/Library Checker index from the repository."""

from collections import defaultdict
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]


def main():
    headers = sorted((ROOT / "lib").rglob("*.hpp"))
    tests = sorted((ROOT / "tests/library_checker").rglob("*.cpp"))
    snippets = sorted((ROOT / "sublime_snippets").rglob("*.sublime-snippet"))
    direct = defaultdict(list)
    for test in tests:
        for include in re.findall(r'^\s*#\s*include\s+"([^"]+)"', test.read_text(), re.MULTILINE):
            direct[include].append(test)
    groups = defaultdict(list)
    for header in headers:
        groups[header.parent.relative_to(ROOT / "lib").as_posix()].append(header)
    lines = [
        "# Library catalog", "",
        f"{len(headers)} headers, {len(headers)} module documents, {len(snippets)} Sublime Text snippets, "
        f"and {len(tests)} Library Checker programs. The example column lists direct header includes; "
        "it does not indicate complete correctness coverage.", "",
    ]
    for group, files in groups.items():
        lines.extend([f"## {group}", "", "| Header | Documentation | Snippet | Direct include examples |",
                      "| --- | --- | --- | --- |"])
        for header in files:
            rel = header.relative_to(ROOT / "lib")
            doc = Path("docs") / rel.with_suffix(".md")
            if not (ROOT / doc).is_file():
                raise RuntimeError(f"missing module documentation: {doc}")
            snippet = Path("sublime_snippets") / rel.with_suffix(".sublime-snippet")
            snippet_link = f"[Snippet](../{snippet.as_posix()})" if (ROOT / snippet).is_file() else "-"
            examples = "<br>".join(
                f"[{p.relative_to(ROOT / 'tests/library_checker').as_posix()}]"
                f"(../{p.relative_to(ROOT).as_posix()})"
                for p in direct[header.relative_to(ROOT).as_posix()]
            ) or "-"
            lines.append(f"| [{header.name}](../lib/{rel.as_posix()}) "
                         f"| [Documentation]({rel.with_suffix('.md').as_posix()}) | {snippet_link} | {examples} |")
        lines.append("")
    lines.extend(["## Library Checker programs", "", "| Category | Count | Directory |", "| --- | ---: | --- |"])
    test_groups = defaultdict(list)
    for test in tests:
        test_groups[test.parent.relative_to(ROOT / "tests/library_checker").as_posix()].append(test)
    for group, files in test_groups.items():
        lines.append(f"| {group} | {len(files)} | [Browse](../tests/library_checker/{group}/) |")
    lines.extend(["", "The first line of each test source links to its problem. See the "
                  "[benchmark guide](../benchmarks/README.md) for official test data and reproducible measurements.", ""])
    (ROOT / "docs/catalog.md").write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
