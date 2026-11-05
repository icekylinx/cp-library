#!/usr/bin/env python3
"""Index completed benchmark records and their current-source header consumers."""

from collections import defaultdict
from datetime import datetime
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def library_digest():
    digest = hashlib.sha256()
    for path in sorted((ROOT / "lib").rglob("*.hpp")):
        digest.update(path.relative_to(ROOT).as_posix().encode() + b"\0" + path.read_bytes() + b"\0")
    return digest.hexdigest()


def dependencies(source):
    found, pending = set(), [source]
    while pending:
        path = pending.pop()
        for include in re.findall(r'^\s*#\s*include\s+"(lib/[^\"]+)"', path.read_text(), re.MULTILINE):
            if include not in found:
                found.add(include)
                pending.append(ROOT / include)
    return found


def main():
    config = json.loads((ROOT / "benchmarks/problems.json").read_text())
    records = {}
    for path in sorted((ROOT / "benchmarks/results").glob("*.json")):
        report = json.loads(path.read_text())
        if "finished_at" not in report:
            continue
        timestamp = datetime.fromisoformat(report["finished_at"])
        for problem in report["problems"]:
            key = (timestamp, path.name)
            if problem["id"] not in records or key > records[problem["id"]][0]:
                records[problem["id"]] = (key, report, problem, path)
    digest = library_digest()
    current, consumers = {}, defaultdict(list)
    for problem_id, source in config.items():
        entry = records.get(problem_id)
        if entry is None:
            continue
        _, report, problem, _ = entry
        compatible = (
            report["library_sources_sha256"] == digest
            and problem["source"] == source
            and problem["source_sha256"] == sha256(ROOT / source)
        )
        current[problem_id] = compatible
        if compatible and problem["passed_cases"] == problem["total_cases"] and not problem.get("failures"):
            for header in dependencies(ROOT / source):
                consumers[header].append(problem_id)
    accepted = sum(records[p][2]["passed_cases"] for p in config if current.get(p))
    total = sum(records[p][2]["total_cases"] for p in config if current.get(p))
    successful = sum(current.get(p, False) and records[p][2]["passed_cases"] == records[p][2]["total_cases"]
                     for p in config if p in records)
    timed = sum(len(records[p][2]["benchmarks"]) for p in config if current.get(p))
    lines = [
        "# Benchmark coverage", "",
        f"{sum(current.values())}/{len(config)} configured programs have completed evaluation records matching "
        f"their current source and all current library headers. {successful} programs passed every registered case: "
        f"{accepted}/{total} official cases passed overall, with {timed} timed inputs from passing programs.", "",
        "This index selects the newest completed record per problem. Source status compares both the program "
        "SHA256 and the full library source digest; a changed source makes a record historical rather than "
        "evidence for the current version. Failed evaluations remain visible and are excluded from timing "
        "and passing-consumer coverage. An unrecorded program remains explicitly unmeasured.", "",
        "## Programs", "",
        "| Problem | Official cases | Timed inputs | Selection | Record | Source status |",
        "| --- | ---: | ---: | --- | --- | --- |",
    ]
    for problem_id in sorted(config):
        if problem_id not in records:
            lines.append(f"| {problem_id} | - | - | - | - | unmeasured |")
            continue
        _, report, problem, path = records[problem_id]
        policy = report.get("case_policy", "largest")
        status = "current" if current[problem_id] else "historical"
        if problem.get("failures") or problem["passed_cases"] != problem["total_cases"]:
            status += "; FAILED"
        lines.append(
            f"| [{problem_id}](../{config[problem_id]}) | {problem['passed_cases']}/{problem['total_cases']} "
            f"| {len(problem['benchmarks'])} | {policy} | [{path.stem}](results/{path.stem}.md) | {status} |"
        )
    lines.extend([
        "", "## Header consumers", "",
        "A listed program includes the header directly or transitively and has a current-source checker record. "
        "This identifies exercised consumers, not exhaustive coverage of all APIs, models, or branches. "
        "A header without a listed program may still have a runnable documentation example.", "",
        "| Header | Current-source official-program consumers |", "| --- | --- |",
    ])
    for path in sorted((ROOT / "lib").rglob("*.hpp")):
        rel = path.relative_to(ROOT / "lib")
        programs = ", ".join(f"`{problem}`" for problem in sorted(consumers[path.relative_to(ROOT).as_posix()]))
        if not programs:
            programs = "unimplemented placeholder" if rel.as_posix() == "math/modular/online_inverse.hpp" else "none recorded"
        lines.append(f"| [{rel.as_posix()}](../docs/{rel.with_suffix('.md').as_posix()}) | {programs} |")
    lines.extend([
        "", "## Rebuild and interpretation", "",
        "Run `python3 tools/update_benchmark_index.py` after adding a completed report or changing sources. "
        "See the [benchmark guide](README.md) for reproduction and the "
        "[verification guide](../docs/guides/verification.md) for input contracts and the limits of checker evidence.", "",
        "Selection policies differ across records. The original record times two largest inputs per problem; "
        "the expanded record prefers three generator families and excludes no-output workloads from timing. "
        "Consult the individual report and input hashes before comparing results.", "",
    ])
    (ROOT / "benchmarks/coverage.md").write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
