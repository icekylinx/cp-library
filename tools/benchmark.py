#!/usr/bin/env python3
"""Check official Library Checker cases, then time selected cases against its solution."""

import argparse
from datetime import datetime, timezone
import fnmatch
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import resource
import shlex
import statistics
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]


class SolutionFailure(RuntimeError):
    def __init__(self, message, returncode, status="runtime_error"):
        super().__init__(message)
        self.returncode = returncode
        self.status = status


def sha256(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def git(directory, *args):
    return subprocess.check_output(["git", "-C", str(directory), *args], text=True).strip()


def compile_source(cxx, flags, source, output, include_dirs):
    command = [cxx, *flags]
    for directory in include_dirs:
        command.extend(["-I", str(directory)])
    command.extend([str(source), "-o", str(output)])
    subprocess.run(command, cwd=ROOT, check=True)
    return command


def run_solution(binary, input_path, output_path, timeout):
    """Both outputs and inputs use real file descriptors, including mmap-compatible stdin."""
    usage_before = resource.getrusage(resource.RUSAGE_CHILDREN)
    with input_path.open("rb") as input_file, output_path.open("wb") as output_file:
        start = time.perf_counter_ns()
        process = subprocess.run(
            [str(binary)], stdin=input_file, stdout=output_file,
            stderr=subprocess.PIPE, timeout=timeout,
        )
        wall_ms = (time.perf_counter_ns() - start) / 1e6
    usage_after = resource.getrusage(resource.RUSAGE_CHILDREN)
    if process.returncode:
        raise SolutionFailure(
            f"{binary.name} failed on {input_path.name}: exit {process.returncode}\n"
            + process.stderr.decode(errors="replace"), process.returncode,
        )
    cpu_ms = 1000 * (
        usage_after.ru_utime + usage_after.ru_stime
        - usage_before.ru_utime - usage_before.ru_stime
    )
    return {"wall_ms": wall_ms, "cpu_ms": cpu_ms}


def verify_data(problem_dir):
    expected = json.loads((problem_dir / "hash.json").read_text())
    paths = sorted((problem_dir / "in").glob("*.in"))
    paths += sorted((problem_dir / "out").glob("*.out"))
    actual = {path.name: sha256(path) for path in paths}
    if actual != expected:
        raise RuntimeError(f"{problem_dir.name}: data differ from upstream hash.json; run --generate")


def select_cases(inputs, count, policy, exclude_patterns):
    eligible = sorted(
        (path for path in inputs if not any(fnmatch.fnmatchcase(path.name, pattern) for pattern in exclude_patterns)),
        key=lambda path: (-path.stat().st_size, path.name),
    )
    if not eligible:
        raise RuntimeError("no benchmark inputs remain after case exclusions")
    if policy == "largest":
        return eligible[:count]
    selected, groups = [], set()
    for path in eligible:
        group = re.sub(r"_\d+$", "", path.stem)
        if group not in groups:
            selected.append(path)
            groups.add(group)
        if len(selected) == count:
            return selected
    return (selected + [path for path in eligible if path not in selected])[:count]


def timing_summary(samples):
    values = [sample["wall_ms"] for sample in samples]
    return f"{statistics.median(values):.3f} [{min(values):.3f}, {max(values):.3f}]"


def render_report(report, output_path):
    policy = report.get("case_policy", "largest")
    selection = (
        "prefer the largest input from each generator family, then fill remaining slots by size"
        if policy == "diverse" else "select the largest inputs by byte size"
    )
    lines = [
        "# Local Library Checker benchmark", "",
        f"Run started: {report['started_at']}.", "",
        f"Upstream: [yosupo06/library-checker-problems](https://github.com/yosupo06/library-checker-problems), "
        f"commit [`{report['upstream_commit'][:12]}`]"
        f"(https://github.com/yosupo06/library-checker-problems/tree/{report['upstream_commit']}).", "",
        "## Methodology", "",
        f"- CPU: {report['cpu_model']}; pinned to logical CPU {report['cpu']}.",
        f"- Platform: `{report['platform']}`.",
        f"- Compiler: `{report['compiler']}`; both programs use the command-line flags `{' '.join(report['flags'])}`.",
        "- Source-level GCC optimize/target directives are retained. Equal command-line flags do not imply equal effective optimization settings; per-problem directives are recorded in the JSON.",
        "- Generate every official case, verify inputs and expected outputs against upstream SHA256 hashes, and check the library program on every case with the official checker.",
        f"- For each problem, {selection}, up to {report['cases_per_problem']} cases. Break size ties by filename.",
        f"- Exclude these filename patterns from timing only: `{', '.join(report.get('exclude_case', [])) or '(none)'}`. Every official case still participates in correctness checks.",
        f"- Warm up each program {report['warmups']} time(s) per case, then measure {report['repetitions']} runs; alternate which program runs first.",
        "- Read stdin from regular files and send stdout to `/dev/null`. The table reports median wall-clock time, including process startup, input parsing, and output formatting; compilation, generation, and checking are excluded.",
        "- File caches are warm, the CPU is shared, and frequency may vary. These results compare complete programs on this machine, not current online rankings or isolated data-structure operations.", "",
        "## Results", "",
        "The ratio is reference time divided by library time; values above 1 mean the library was faster on that case. Programs with any verification failure are not timed.", "",
        "Each timing cell is the median [minimum, maximum] in milliseconds. The range describes observed run-to-run spread, not a confidence interval.", "",
        "| Problem | Correctness | Timed case | Input MiB | Library ms | Reference ms | Ratio |",
        "| --- | --- | --- | ---: | ---: | ---: | ---: |",
    ]
    for problem in report["problems"]:
        if problem.get("failures"):
            lines.append(f"| {problem['id']} | {problem['passed_cases']}/{problem['total_cases']} FAIL | - | - | - | - | - |")
        for case in problem["benchmarks"]:
            candidate = statistics.median(x["wall_ms"] for x in case["candidate"])
            reference = statistics.median(x["wall_ms"] for x in case["reference"])
            lines.append(
                f"| [{problem['id']}](https://judge.yosupo.jp/problem/{problem['id']}) "
                f"| {problem['passed_cases']}/{problem['total_cases']} "
                f"| `{case['case']}` | {case['input_bytes'] / 2**20:.2f} "
                f"| {timing_summary(case['candidate'])} | {timing_summary(case['reference'])} | {reference / candidate:.2f}x |"
            )
    total = sum(p["total_cases"] for p in report["problems"])
    passed = sum(p["passed_cases"] for p in report["problems"])
    failed = [p for p in report["problems"] if p.get("failures")]
    lines.extend([
        "", f"{passed}/{total} official cases passed the checker across {len(report['problems'])} problems; "
        f"{len(failed)} program(s) had verification failures.", "",
    ])
    if failed:
        lines.extend(["## Verification failures", "",
                      "Failed programs were not timed. Algorithm and solution sources were preserved. "
                      "Each failure below was also checked with the pinned official input verifier.", "",
                      "| Problem | Case | Failure | Input verifier exit | Input SHA256 |",
                      "| --- | --- | --- | ---: | --- |"])
        for problem in failed:
            for failure in problem["failures"]:
                lines.append(f"| {problem['id']} | `{failure['case']}` | {failure['status']} "
                             f"(exit {failure.get('returncode', 'n/a')}) | {failure['input_verifier_exit']} "
                             f"| `{failure['input_sha256']}` |")
        lines.append("")
    lines.extend([
        "## Reproduction and raw data", "",
        "See the [benchmark guide](../README.md) for commands. The matching JSON file contains every wall-clock "
        "and CPU-time sample, compilation commands, input and source hashes, and case verification records.", "",
        f"Library source digest for this run: `{report['library_sources_sha256']}`.", "",
    ])
    output_path.write_text("\n".join(lines), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--upstream", type=Path, default=ROOT / "local/library-checker-problems")
    parser.add_argument("--problems", nargs="+", help="Subset of benchmarks/problems.json")
    parser.add_argument("--generate", action="store_true", help="Run upstream generate.py first")
    parser.add_argument("--keep-going", action="store_true",
                        help="Record verification failures and continue; never time a failing program")
    parser.add_argument("--cases", type=int, default=3)
    parser.add_argument("--case-policy", choices=("diverse", "largest"), default="diverse")
    parser.add_argument("--exclude-case", action="append", default=[],
                        help="Additional filename glob excluded from timing, not verification")
    parser.add_argument("--include-no-output", action="store_true",
                        help="Allow no_output* timing cases, as in the original benchmark policy")
    parser.add_argument("--repetitions", type=int, default=5)
    parser.add_argument("--warmups", type=int, default=1)
    parser.add_argument("--timeout", type=float, default=30)
    parser.add_argument("--cpu", type=int)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--flags", default="-std=c++23 -O2")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if not args.include_no_output:
        args.exclude_case = ["no_output*", *args.exclude_case]
    if args.cases < 1 or args.repetitions < 1 or args.warmups < 0 or args.timeout <= 0:
        parser.error("cases, repetitions and timeout must be positive; warmups must be nonnegative")
    if not hasattr(os, "sched_setaffinity"):
        parser.error("benchmark requires Linux CPU affinity support")
    available = os.sched_getaffinity(0)
    cpu = args.cpu if args.cpu is not None else min(available)
    if cpu not in available:
        parser.error(f"CPU {cpu} not in allowed CPUs {sorted(available)}")
    upstream = args.upstream.resolve()
    if not (upstream / "generate.py").is_file():
        parser.error("clone yosupo06/library-checker-problems into local/library-checker-problems first")
    if git(upstream, "status", "--porcelain", "--untracked-files=no"):
        parser.error("upstream tracked files have local edits; use a clean upstream checkout")
    config = json.loads((ROOT / "benchmarks/problems.json").read_text())
    selected = args.problems if args.problems else list(config)
    unknown = set(selected) - set(config)
    if unknown:
        parser.error(f"unknown problem IDs: {sorted(unknown)}")
    flags = shlex.split(args.flags)
    timestamp = datetime.now(timezone.utc)
    output = args.output or ROOT / "benchmarks/results" / (timestamp.strftime("%Y-%m-%d") + ".json")
    output = output.resolve()
    if output.suffix != ".json":
        parser.error("--output must end with .json")
    if output.exists() or output.with_suffix(".md").exists():
        parser.error("output already exists; choose a new --output to preserve prior runs")
    output.parent.mkdir(parents=True, exist_ok=True)
    work = ROOT / "build/benchmark" / timestamp.strftime("%Y%m%dT%H%M%S%fZ")
    work.mkdir(parents=True)
    resource.setrlimit(resource.RLIMIT_STACK, (resource.RLIM_INFINITY, resource.RLIM_INFINITY))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if args.generate:
        env = dict(os.environ, CXX=args.cxx, CXXFLAGS=" ".join(flags))
        with (work / "generation.log").open("w") as log:
            subprocess.run([sys.executable, str(upstream / "generate.py"), "-p", *selected],
                           stdout=log, stderr=subprocess.STDOUT, env=env, check=True)
    os.sched_setaffinity(0, {cpu})
    digest = hashlib.sha256()
    for path in sorted((ROOT / "lib").rglob("*.hpp")):
        digest.update(path.relative_to(ROOT).as_posix().encode() + b"\0" + path.read_bytes() + b"\0")
    cpu_info = Path("/proc/cpuinfo").read_text()
    model = re.search(r"^model name\s*:\s*(.+)$", cpu_info, re.MULTILINE)
    report = {
        "schema_version": 2, "runner_sha256": sha256(Path(__file__)),
        "started_at": timestamp.isoformat(), "platform": platform.platform(),
        "cpu_model": model.group(1) if model else platform.processor(), "cpu": cpu,
        "compiler": subprocess.check_output([args.cxx, "--version"], text=True).splitlines()[0],
        "python": platform.python_version(), "flags": flags,
        "upstream_repository": "https://github.com/yosupo06/library-checker-problems",
        "upstream_commit": git(upstream, "rev-parse", "HEAD"),
        "library_base_commit": git(ROOT, "rev-parse", "HEAD"),
        "library_sources_sha256": digest.hexdigest(), "cases_per_problem": args.cases,
        "repetitions": args.repetitions, "warmups": args.warmups,
        "case_policy": args.case_policy, "exclude_case": args.exclude_case,
        "keep_going": args.keep_going, "problems": [],
    }
    for problem_id in selected:
        print(f"Checking {problem_id}...", flush=True)
        matches = list(upstream.glob(f"*/{problem_id}/info.toml"))
        if len(matches) != 1:
            raise RuntimeError(f"expected exactly one upstream {problem_id}/info.toml")
        problem_dir = matches[0].parent
        verify_data(problem_dir)
        source = ROOT / config[problem_id]
        reference_source = problem_dir / "sol/correct.cpp"
        candidate, reference = work / problem_id, work / (problem_id + "-reference")
        candidate_command = compile_source(args.cxx, flags, source, candidate, [ROOT])
        reference_command = compile_source(args.cxx, flags, reference_source, reference, [upstream / "common"])
        inputs = sorted((problem_dir / "in").glob("*.in"))
        actual_path = work / (problem_id + ".actual")
        checks = []
        failures = []
        verifier = work / (problem_id + "-input-verifier")
        verifier_command = None
        for input_path in inputs:
            expected_path = problem_dir / "out" / (input_path.stem + ".out")
            check = {"case": input_path.name, "input_sha256": sha256(input_path),
                     "expected_sha256": sha256(expected_path)}
            try:
                run_solution(candidate, input_path, actual_path, args.timeout)
                checked = subprocess.run(
                    [str(problem_dir / "checker"), str(input_path), str(actual_path), str(expected_path)],
                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=args.timeout,
                )
                if checked.returncode:
                    raise SolutionFailure("checker rejected output: " + checked.stderr.decode(errors="replace"),
                                          checked.returncode, "checker_rejection")
                check.update(status="passed", checker_exit=0)
            except (SolutionFailure, subprocess.TimeoutExpired) as error:
                if not args.keep_going:
                    raise
                if not verifier.exists():
                    verifier_command = compile_source(args.cxx, flags, problem_dir / "verifier.cpp", verifier,
                                                      [upstream / "common"])
                with input_path.open("rb") as stream:
                    validity = subprocess.run([str(verifier)], stdin=stream, stdout=subprocess.PIPE,
                                              stderr=subprocess.PIPE, timeout=args.timeout)
                check.update(status="timeout" if isinstance(error, subprocess.TimeoutExpired) else error.status,
                             returncode=getattr(error, "returncode", None), error=str(error),
                             input_verifier_exit=validity.returncode,
                             input_verifier_message=validity.stderr.decode(errors="replace"))
                failures.append(check)
            checks.append(check)
        result = {
            "id": problem_id, "source": config[problem_id], "source_sha256": sha256(source),
            "reference_source_sha256": sha256(reference_source),
            "candidate_pragmas": re.findall(r"^#pragma GCC .+$", source.read_text(), re.MULTILINE),
            "reference_pragmas": re.findall(r"^#pragma GCC .+$", reference_source.read_text(), re.MULTILINE),
            "candidate_compile": candidate_command, "reference_compile": reference_command,
            "upstream_hash_manifest_sha256": sha256(problem_dir / "hash.json"),
            "problem_contract_sha256": {
                name: sha256(problem_dir / name)
                for name in ("task.md", "verifier.cpp", "info.toml")
            },
            "status": "failed" if failures else "passed", "failures": failures,
            "input_verifier_compile": verifier_command,
            "passed_cases": len(checks) - len(failures), "total_cases": len(inputs), "checks": checks, "benchmarks": [],
        }
        largest = [] if failures else select_cases(inputs, args.cases, args.case_policy, args.exclude_case)
        for input_path in largest:
            for _ in range(args.warmups):
                for binary in (candidate, reference):
                    run_solution(binary, input_path, Path(os.devnull), args.timeout)
            times = {"candidate": [], "reference": []}
            for repetition in range(args.repetitions):
                order = [("candidate", candidate), ("reference", reference)]
                if repetition % 2:
                    order.reverse()
                for name, binary in order:
                    times[name].append(run_solution(binary, input_path, Path(os.devnull), args.timeout))
            result["benchmarks"].append({
                "case": input_path.name, "input_bytes": input_path.stat().st_size,
                "input_sha256": sha256(input_path), **times,
            })
        report["problems"].append(result)
        print(f"{result['status'].upper()} {problem_id}: {result['passed_cases']}/{len(checks)} checked; "
              f"{len(largest)} benchmark cases", flush=True)
    report["finished_at"] = datetime.now(timezone.utc).isoformat()
    report["status"] = "completed_with_failures" if any(p.get("failures") for p in report["problems"]) else "passed"
    output.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    render_report(report, output.with_suffix(".md"))
    print(f"Saved {output} and {output.with_suffix('.md')}", flush=True)
    if report["status"] != "passed":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
