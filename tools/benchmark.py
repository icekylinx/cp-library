#!/usr/bin/env python3
"""Check official Library Checker cases, then time selected cases against its solution."""

import argparse
from datetime import datetime, timezone
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
        raise RuntimeError(
            f"{binary.name} failed on {input_path.name}: exit {process.returncode}\n"
            + process.stderr.decode(errors="replace")
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


def render_report(report, output_path):
    lines = [
        "# Library Checker 本地基准测试", "",
        f"运行时间：{report['started_at']}。", "",
        f"上游：[yosupo06/library-checker-problems](https://github.com/yosupo06/library-checker-problems) "
        f"的提交 [`{report['upstream_commit'][:12]}`]"
        f"(https://github.com/yosupo06/library-checker-problems/tree/{report['upstream_commit']})。", "",
        "## 测量方法", "",
        f"- CPU：{report['cpu_model']}；固定逻辑 CPU {report['cpu']}。",
        f"- 系统：`{report['platform']}`。",
        f"- 编译器：`{report['compiler']}`；两份程序的命令行参数均为 `{' '.join(report['flags'])}`。",
        "- 保留源码中的 GCC optimize/target 指令，因此相同命令行参数不表示相同的有效优化级别；逐题指令记录在 JSON 中。",
        "- 每题完整生成官方数据，逐项校验上游 SHA256，再用官方 checker 检查本库程序的所有用例。",
        f"- 每题按输入文件字节数降序、文件名升序，取前 {report['cases_per_problem']} 个用例计时。",
        f"- 每份程序每例预热 {report['warmups']} 次，测量 {report['repetitions']} 次；交替先运行本库与参考解。",
        "- 输入为普通文件，输出到 `/dev/null`。表中为包含进程启动、输入解析及输出格式化的墙钟耗时中位数；不含编译、生成及 checker。",
        "- 文件缓存经过预热，CPU 未独占，频率可动态变化。该结果只比较本机当前程序，不代表在线排名或纯数据结构操作耗时。", "",
        "## 结果", "",
        "比值为官方参考解耗时 ÷ 本库耗时；大于 1 表示本库在该用例中更快。", "",
        "| 题目 | 正确性 | 计时用例 | 输入 MiB | 本库 ms | 官方参考 ms | 比值 |",
        "| --- | --- | --- | ---: | ---: | ---: | ---: |",
    ]
    for problem in report["problems"]:
        for case in problem["benchmarks"]:
            candidate = statistics.median(x["wall_ms"] for x in case["candidate"])
            reference = statistics.median(x["wall_ms"] for x in case["reference"])
            lines.append(
                f"| [{problem['id']}](https://judge.yosupo.jp/problem/{problem['id']}) "
                f"| {problem['passed_cases']}/{problem['total_cases']} "
                f"| `{case['case']}` | {case['input_bytes'] / 2**20:.2f} "
                f"| {candidate:.3f} | {reference:.3f} | {reference / candidate:.2f}× |"
            )
    total = sum(p["total_cases"] for p in report["problems"])
    lines.extend([
        "", f"共 {len(report['problems'])} 题、{total} 个官方用例通过 checker。", "",
        "## 复现与原始数据", "",
        "执行方法见 [benchmark 使用说明](../README.md)。同名 JSON 保存所有单次墙钟与 CPU 时间、"
        "编译命令、输入及源码哈希、官方用例验证记录。", "",
        f"本次库源码摘要：`{report['library_sources_sha256']}`。", "",
    ])
    output_path.write_text("\n".join(lines), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--upstream", type=Path, default=ROOT / "local/library-checker-problems")
    parser.add_argument("--problems", nargs="+", help="Subset of benchmarks/problems.json")
    parser.add_argument("--generate", action="store_true", help="Run upstream generate.py first")
    parser.add_argument("--cases", type=int, default=2)
    parser.add_argument("--repetitions", type=int, default=5)
    parser.add_argument("--warmups", type=int, default=1)
    parser.add_argument("--timeout", type=float, default=30)
    parser.add_argument("--cpu", type=int)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--flags", default="-std=c++23 -O2")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
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
        "started_at": timestamp.isoformat(), "platform": platform.platform(),
        "cpu_model": model.group(1) if model else platform.processor(), "cpu": cpu,
        "compiler": subprocess.check_output([args.cxx, "--version"], text=True).splitlines()[0],
        "python": platform.python_version(), "flags": flags,
        "upstream_repository": "https://github.com/yosupo06/library-checker-problems",
        "upstream_commit": git(upstream, "rev-parse", "HEAD"),
        "library_base_commit": git(ROOT, "rev-parse", "HEAD"),
        "library_sources_sha256": digest.hexdigest(), "cases_per_problem": args.cases,
        "repetitions": args.repetitions, "warmups": args.warmups, "problems": [],
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
        for input_path in inputs:
            expected_path = problem_dir / "out" / (input_path.stem + ".out")
            run_solution(candidate, input_path, actual_path, args.timeout)
            checked = subprocess.run(
                [str(problem_dir / "checker"), str(input_path), str(actual_path), str(expected_path)],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=args.timeout,
            )
            if checked.returncode:
                raise RuntimeError(f"{problem_id}/{input_path.name}: checker rejected output\n"
                                   + checked.stderr.decode(errors="replace"))
            checks.append({"case": input_path.name, "input_sha256": sha256(input_path),
                           "expected_sha256": sha256(expected_path), "checker_exit": 0})
        result = {
            "id": problem_id, "source": config[problem_id], "source_sha256": sha256(source),
            "reference_source_sha256": sha256(reference_source),
            "candidate_pragmas": re.findall(r"^#pragma GCC .+$", source.read_text(), re.MULTILINE),
            "reference_pragmas": re.findall(r"^#pragma GCC .+$", reference_source.read_text(), re.MULTILINE),
            "candidate_compile": candidate_command, "reference_compile": reference_command,
            "upstream_hash_manifest_sha256": sha256(problem_dir / "hash.json"),
            "passed_cases": len(checks), "total_cases": len(inputs), "checks": checks, "benchmarks": [],
        }
        largest = sorted(inputs, key=lambda p: (-p.stat().st_size, p.name))[:args.cases]
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
        print(f"PASS {problem_id}: {len(checks)} checked; {len(largest)} benchmark cases", flush=True)
    report["finished_at"] = datetime.now(timezone.utc).isoformat()
    output.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    render_report(report, output.with_suffix(".md"))
    print(f"Saved {output} and {output.with_suffix('.md')}", flush=True)


if __name__ == "__main__":
    main()
