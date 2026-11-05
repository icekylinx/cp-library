# Library Checker benchmark

用官方生成器和 checker 检查本库程序，再与同版本官方 `sol/correct.cpp` 比较本机耗时。默认选题见 [problems.json](problems.json)。

## 已完成的测量

- [2026 年 11 月 5 日结果](results/2026-11-05.md)：8 题、158 个官方用例验证通过；16 个大输入用例各预热 1 次、测量 5 次。
- [原始数据](results/2026-11-05.json)：保存每次耗时、编译参数、源码和输入哈希及 checker 记录。
- 本次上游版本为 [`73876f1cbcf082d2b22722924b924d3d65a2f00e`](https://github.com/yosupo06/library-checker-problems/tree/73876f1cbcf082d2b22722924b924d3d65a2f00e)，于 2026 年 11 月 5 日从官方默认分支拉取。

## 下载或更新生成器

官方题目仓库包含各题 `gen/`、校验器、参考解及登记的测试哈希，保存在 Git 忽略的 `local/` 中，无需把生成器或大数据提交到本库。

```sh
mkdir -p local
git clone --depth 1 https://github.com/yosupo06/library-checker-problems.git local/library-checker-problems
```

已有克隆时更新：

```sh
git -C local/library-checker-problems pull --ff-only
git -C local/library-checker-problems rev-parse HEAD
```

复现旧报告应使用报告中固定的上游提交；更新后重新生成数据，会得到新的测量版本。官方生成器自带 TOML 解析器；`colorlog` 是可选的彩色日志依赖。

## 生成、验证与测量

需要 Linux、Python 3.11 或更新版本以及支持 C++23 的 GCC。部分库实现需要 AVX2，本次机器支持该指令集。所有命令在项目根目录执行。

```sh
python3 tools/benchmark.py --generate
```

默认运行 8 题。程序先生成完整官方用例，再检查输入与预期输出的 SHA256，运行本库程序并逐例调用官方 checker；任一环节失败便停止，不输出成功报告。计时前重新编译双方程序，选择每题按字节数最大的两个输入；相同大小按文件名排序。

已生成数据时可跳过生成步骤：

```sh
python3 tools/benchmark.py --problems unionfind staticrmq --output benchmarks/results/local-comparison.json
```

默认固定在当前可用的最小逻辑 CPU 编号；用 `--cpu` 改选。默认预热 1 次、测量 5 次，可用 `--warmups`、`--repetitions`、`--cases` 调整。每份程序单例默认超时 30 秒，可用 `--timeout` 调整。默认参数是 `-std=c++23 -O2`，可通过 `--flags='-std=c++23 -O3'` 修改。

报告默认写入 `benchmarks/results/UTC日期.json` 与同名 Markdown。已有同名报告时拒绝覆盖；同一天再次运行请指定新的 `--output`。中间可执行文件、checker 输出和生成日志位于 `build/benchmark/`。

上游树直径参考解采用递归，长链数据需要较大栈。此工具将子进程栈限制设为 unlimited；若直接使用官方生成器，可按[官方说明](https://github.com/yosupo06/library-checker-problems#how-to-use)先执行 `ulimit -s unlimited`。

## 如何理解数据

测量的是完整程序的墙钟时间，含启动、输入解析、计算和输出格式化。输入通过普通文件读取，适配本库默认 mmap，输出写入 `/dev/null`；checker 不参与计时。双方的预热和输出方式一致，每轮交替运行顺序。报告同时保留 CPU 时间，但摘要表采用墙钟中位数。

同样的命令行参数不消除源码中的 GCC optimize/target 指令差异，JSON 保留这些指令。CPU 未独占，系统负载和动态频率仍会影响结果。该比较不等于只测算法内核，也不能换算为 Library Checker 当前在线排名；`no_output` 用例主要衡量其对应的无查询工作负载。
