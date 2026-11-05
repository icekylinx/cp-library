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
        "# 算法库目录", "",
        f"共 {len(headers)} 个头文件、{len(headers)} 篇模块文档、{len(snippets)} 个 Sublime Text 代码片段、"
        f"{len(tests)} 个 Library Checker 程序。示例栏列出直接包含头文件的程序，不代表完整正确性覆盖。", "",
    ]
    for group, files in groups.items():
        lines.extend([f"## {group}", "", "| 头文件 | 文档 | 代码片段 | 直接引用示例 |",
                      "| --- | --- | --- | --- |"])
        for header in files:
            rel = header.relative_to(ROOT / "lib")
            doc = Path("docs") / rel.with_suffix(".md")
            if not (ROOT / doc).is_file():
                raise RuntimeError(f"missing module documentation: {doc}")
            snippet = Path("sublime_snippets") / rel.with_suffix(".sublime-snippet")
            snippet_link = f"[片段](../{snippet.as_posix()})" if (ROOT / snippet).is_file() else "—"
            examples = "<br>".join(
                f"[{p.relative_to(ROOT / 'tests/library_checker').as_posix()}]"
                f"(../{p.relative_to(ROOT).as_posix()})"
                for p in direct[header.relative_to(ROOT).as_posix()]
            ) or "—"
            lines.append(f"| [{header.name}](../lib/{rel.as_posix()}) "
                         f"| [文档]({rel.with_suffix('.md').as_posix()}) | {snippet_link} | {examples} |")
        lines.append("")
    lines.extend(["## Library Checker 程序", "", "| 分类 | 数量 | 路径 |", "| --- | ---: | --- |"])
    test_groups = defaultdict(list)
    for test in tests:
        test_groups[test.parent.relative_to(ROOT / "tests/library_checker").as_posix()].append(test)
    for group, files in test_groups.items():
        lines.append(f"| {group} | {len(files)} | [浏览](../tests/library_checker/{group}/) |")
    lines.extend(["", "测试源文件首行保留对应题目链接。完整评测及性能复现见 [benchmark 说明](../benchmarks/README.md)。", ""])
    (ROOT / "docs/catalog.md").write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
