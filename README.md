# cp-library

面向算法竞赛的 C++ 算法库，包含 38 个头文件、38 篇模块文档、31 个 Library Checker 程序和 37 个 Sublime Text 代码片段。采用 MIT 许可证。

## 目录与文档

| 路径 | 内容 |
| --- | --- |
| [lib/](lib/) | 算法、数据结构、数学、字符串、树及工具头文件 |
| [tests/](tests/) | 仅保留 Library Checker 的评测程序 |
| [sublime_snippets/](sublime_snippets/) | 按模块分类的 Sublime Text 代码片段 |
| [docs/catalog.md](docs/catalog.md) | 全部模块文档、头文件、代码片段与示例索引 |
| [docs/usage.md](docs/usage.md) | 原有英文使用说明 |
| [benchmarks/README.md](benchmarks/README.md) | 官方生成器、本地验证与性能复现说明 |
| [benchmarks/results/2026-11-05.md](benchmarks/results/2026-11-05.md) | 8 题本地 benchmark 汇总与参考解对比 |
| [docs/benchmarks.md](docs/benchmarks.md) | 原 README 中的历史性能记录及提交链接 |
| [template.cpp](template.cpp) | 竞赛程序模板 |
| [expander.py](expander.py) | 将 `lib/` 中的引用展开为单文件 |
| `local/` | 本地题目生成器、调试源码及归档，已由 Git 忽略 |
| `build/` | 本地编译与展开输出，已由 Git 忽略 |

## 快速开始

以下命令均在项目根目录运行。需要支持 C++20 或更新标准的编译器，通常使用 GCC 与 `-std=c++23 -O2`；部分实现使用 GNU 扩展或平台相关指令。

编译并运行现有并查集示例：

```sh
mkdir -p build
g++ -std=c++23 -O2 -I. tests/library_checker/data_structure/unionfind.cpp -o build/unionfind
printf '4 5\n0 0 1\n1 0 1\n1 0 2\n0 1 2\n1 0 2\n' > build/unionfind.in
./build/unionfind < build/unionfind.in
```

输出应为三行：`1`、`0`、`1`。

FastIO 在 Unix 下默认使用 `mmap` 读取普通文件，因此示例采用文件重定向。使用管道或终端输入时，编译时添加 `-DDISABLE_MMAP`，启用 `fread` 缓冲读取。

新程序可从 `template.cpp` 复制至 `local/`，并通过项目根目录下的 `lib/...` 路径包含头文件：

```sh
mkdir -p local
cp template.cpp local/main.cpp
g++ -std=c++23 -O2 -I. local/main.cpp -o build/main
```

模板与现有评测程序在定义 `LOCAL` 时使用当前工作目录中的 `test.in` 和 `test.out`；使用标准输入输出时无需添加 `-DLOCAL`。

## 展开为单文件

```sh
python3 expander.py tests/library_checker/data_structure/unionfind.cpp -o build/unionfind.cpp
g++ -std=c++23 -O2 build/unionfind.cpp -o build/unionfind-expanded
```

`expander.py` 默认从当前目录解析项目路径；从其他目录调用时使用 `--root` 指定项目根目录。更多参数见 `python3 expander.py --help`。

## 使用约定

- 数据结构默认使用从 0 开始的下标与左闭右开区间 `[left, right)`。
- 尺寸通常由 `uint32_t` 表示；零尺寸的行为需查看具体实现。
- 库仅做有限的参数检查，调用前应确保输入满足对应实现的要求。
- Sublime Text 片段和头文件分别维护；使用前请核对需要的版本。

## 文档与验证

每个头文件都有对应的 `docs/模块路径/文件名.md`，说明使用条件、公开接口、复杂度和示例。`online_inverse.hpp` 目前仍是占位文件，文档明确标注为未实现。

```sh
python3 tools/update_catalog.py
python3 tools/check_docs.py
```

第一条重建模块索引；第二条检查头文件的文档覆盖、内部链接，并编译运行标记为可独立运行的 C++ 示例。完整官方数据的生成、checker 验证和计时方法见 [benchmark 说明](benchmarks/README.md)。

## 本地 benchmark

[2026 年 11 月 5 日测量](benchmarks/results/2026-11-05.md)覆盖 8 题，全部 158 个官方用例通过 checker；另选每题两个大输入，记录本库程序与官方参考解的 5 次运行耗时。原始 JSON 包含源码、输入及上游版本哈希。历史在线性能数据单独保留在 [历史记录](docs/benchmarks.md)。

## 许可证

[MIT License](LICENSE)
