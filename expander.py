#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
expander.py

Written by ChatGPT (GPT-5.2 Thinking).

Behavior:
- Expand only quote-includes that resolve inside lib/... into one file.
- Ignore all '#pragma once' lines (remove them from output).
- Preserve original comments from source files; do NOT add any extra comments to generated C++ output.
- When an include expands a new file, insert a blank line after the expanded chunk.
- After all operations:
  - strip trailing spaces/tabs at end of each line
  - compress multiple consecutive blank lines to at most ONE blank line
  - remove trailing blank lines at EOF
  - DO NOT force a final newline
"""

import argparse
import re
import sys
from pathlib import Path

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^">]+)\1\s*$')
PRAGMA_ONCE_RE = re.compile(r'^\s*#\s*pragma\s+once(?:\s*//.*)?\s*$')


def is_within(child: Path, parent: Path) -> bool:
    try:
        child.resolve().relative_to(parent.resolve())
        return True
    except Exception:
        return False


def read_text(p: Path) -> str:
    return p.read_text(encoding="utf-8", errors="replace")


def add_blank_line(out_lines: list[str]) -> None:
    if out_lines and out_lines[-1] != "":
        out_lines.append("")


def expand_file(
    file_path: Path,
    *,
    project_root: Path,
    lib_dir: Path,
    included: set[Path],
    stack: list[Path],
) -> list[str]:
    file_path = file_path.resolve()

    if file_path in included:
        return []

    if file_path in stack:
        cycle = " -> ".join(str(p) for p in stack + [file_path])
        raise RuntimeError(f"Include cycle detected:\n{cycle}")

    if not file_path.exists():
        raise FileNotFoundError(f"File not found: {file_path}")

    included.add(file_path)
    stack.append(file_path)

    out: list[str] = []
    base_dir = file_path.parent

    for raw_line in read_text(file_path).splitlines():
        if PRAGMA_ONCE_RE.match(raw_line):
            continue

        m = INCLUDE_RE.match(raw_line)
        if not m:
            out.append(raw_line)
            continue

        bracket, inc = m.group(1), m.group(2).strip()

        if bracket == "<":
            out.append(raw_line)
            continue

        cand = (base_dir / inc).resolve()
        if not cand.exists():
            cand = (project_root / inc).resolve()

        if cand.exists() and is_within(cand, lib_dir):
            inlined = expand_file(
                cand,
                project_root=project_root,
                lib_dir=lib_dir,
                included=included,
                stack=stack,
            )
            if inlined:
                out.extend(inlined)
                add_blank_line(out)
        else:
            out.append(raw_line)

    stack.pop()
    return out


def postprocess(lines: list[str]) -> str:
    # 1) strip trailing spaces/tabs
    lines = [ln.rstrip(" \t") for ln in lines]

    # 2) compress multiple blank lines to at most one
    out: list[str] = []
    blank_run = 0
    for ln in lines:
        if ln == "":
            blank_run += 1
            if blank_run == 1:
                out.append("")
        else:
            blank_run = 0
            out.append(ln)

    # 3) remove trailing blank lines at EOF
    while out and out[-1] == "":
        out.pop()

    # 4) do NOT force final newline
    return "\n".join(out)


def main() -> int:
    ap = argparse.ArgumentParser(
        description=(
            "Inline quote-includes under lib/... into a single C++ file. "
            "Removes #pragma once, preserves original content, normalizes whitespace."
        )
    )
    ap.add_argument("entry", help="Entry .cpp (e.g. test.cpp).")
    ap.add_argument(
        "-o", "--out",
        default="-",
        help="Output file (default: stdout). Use '-' for stdout."
    )
    ap.add_argument("--root", default=".", help="Project root (default: current dir).")
    ap.add_argument("--lib-dir", default="lib", help="Library directory to inline (default: lib).")
    args = ap.parse_args()

    project_root = Path(args.root).resolve()
    lib_dir = (project_root / args.lib_dir).resolve()
    entry = (project_root / args.entry).resolve()

    if not lib_dir.exists():
        print(f"[expander] ERROR: lib dir not found: {lib_dir}", file=sys.stderr)
        return 1

    if not entry.exists():
        print(f"[expander] ERROR: entry not found: {entry}", file=sys.stderr)
        return 1

    expanded_lines = expand_file(
        entry,
        project_root=project_root,
        lib_dir=lib_dir,
        included=set(),
        stack=[],
    )

    expanded = postprocess(expanded_lines)

    if args.out == "-":
        sys.stdout.write(expanded)
    else:
        out_path = Path(args.out).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(expanded, encoding="utf-8")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())