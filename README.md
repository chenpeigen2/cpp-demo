# cpp-demo —— C23 / C++23 特性与标准库全览

用 **clang** 编译的 C 和 C++ 特性演示集。每个文件独立编译运行、单一主题、中文注释，可作为速查手册。

## 构建（macOS / Apple clang 21）

```sh
make        # 编译全部 68 个演示（C 用 -std=c2x，C++ 用 -std=c++23）
make run    # 编译并依次运行，任何一个失败即报错
make clean
```

要求：Apple clang 21+（随 Xcode Command Line Tools 安装）。

## 目录

| 目录 | 内容 |
|---|---|
| `c/`  | C23 语言特性 + C 标准库头巡礼（26 个文件） |
| `cpp/` | C++23 核心语言 + 标准库头巡礼（42 个文件） |

## C 演示索引

| 文件 | 主题 |
|---|---|
| `01_hello.c` | 程序结构、main、printf |
| `02_types.c` | 基本类型、字面量（二进制/数字分隔符） |
| `03_control.c` | if/switch/循环/goto |
| `04_functions.c` | 原型、递归、函数指针、`[[nodiscard]]` |
| `05_pointers_arrays.c` | 指针算术、二维数组、void* |
| `06_strings.c` | C 字符串本质（'\0' 结尾） |
| `07_struct_union_enum.c` | 结构体、填充、联合、位域、C23 枚举底层类型 |
| `08_memory.c` | malloc/calloc/realloc/free |
| `09_preprocessor.c` | 宏、条件编译、`__VA_ARGS__`、_Pragma |
| `10_operators.c` | 运算符大全、短路求值 |
| `11_storage_scopes.c` | static/extern/const、作用域、VLA |
| `12_generic.c` | C11 `_Generic` 泛型选择 |
| `13_stdarg.c` | 可变参数函数 |
| `14_stdlib.c` | strtol/qsort/bsearch/rand |
| `15_stdio.c` | 文件 I/O、fseek、snprintf |
| `16_string.c` | string.h 内存/字符串函数 |
| `17_math.c` | 数学函数、INFINITY/NAN |
| `18_stdint_limits.c` | 定宽整数、limits/float 边界、inttypes |
| `19_assert_errno.c` | assert、errno、static_assert |
| `20_signal.c` | 信号处理 |
| `21_time.c` | time.h、strftime、clock_gettime |
| `22_ctype_locale.c` | 字符分类、locale |
| `23_complex.c` | 复数运算 |
| `24_atomic.c` | C11 原子操作、CAS、自旋锁 |
| `25_pthreads.c` | POSIX 线程（macOS 无 C11 threads.h） |
| `26_bitwise.c` | 位技巧（popcount/2 的幂/交换） |

## C++ 演示索引

**核心语言**

| 文件 | 主题 |
|---|---|
| `30_hello.cpp` | iostream、std::print |
| `31_references.cpp` | 引用 vs 指针 |
| `32_classes.cpp` | 类、构造/析构、链式调用 |
| `33_special_members.cpp` | 拷贝/移动语义（五法则） |
| `34_operators.cpp` | 运算符重载 |
| `35_inheritance.cpp` | 虚函数、抽象类、dynamic_cast |
| `36_raii.cpp` | RAII 惯用法 |
| `37_templates.cpp` | 函数/类模板、CTAD、编译期递归 |
| `38_templates_advanced.cpp` | 特化、变参模板、折叠表达式 |
| `39_concepts.cpp` | C++20 概念与约束 |

**容器与算法**

| 文件 | 主题 |
|---|---|
| `40_vector.cpp` | vector |
| `41_array_deque_list.cpp` | array/deque/list/forward_list |
| `42_map_set.cpp` | map/set/multimap（有序） |
| `43_unordered.cpp` | unordered_map/set、自定义哈希 |
| `44_flat_adapters.cpp` | C++23 flat_map、stack/queue/priority_queue |
| `45_iterators.cpp` | 迭代器类别与工具 |
| `46_ranges.cpp` | C++20/23 视图管道（zip/adjacent/split） |
| `47_algorithm.cpp` | algorithm 全景 |
| `48_numeric_bit.cpp` | numeric + bit（popcount 等） |
| `49_string.cpp` | string/string_view、C++23 contains |
| `50_regex.cpp` | 正则表达式 |

**智能指针与值语义**

| 文件 | 主题 |
|---|---|
| `51_smart_pointers.cpp` | unique/shared/weak、自定义删除器 |
| `52_exceptions.cpp` | 异常体系、exception_ptr |
| `53_chrono.cpp` | 时钟、duration、C++20 日历 |
| `54_functional.cpp` | function、lambda、bind、hash |
| `55_tuple_pair.cpp` | tuple/pair、结构化绑定、apply |
| `56_optional_variant_any.cpp` | 三种可选类型 + visit |
| `57_expected.cpp` | C++23 expected（monadic 错误处理） |
| `58_format.cpp` | std::format 与自定义 formatter |
| `59_filesystem.cpp` | 文件系统操作 |

**并发与底层**

| 文件 | 主题 |
|---|---|
| `60_threads.cpp` | jthread、mutex、condition_variable、stop_token |
| `61_coroutines.cpp` | 手写 Generator/Lazy（co_yield/co_return/co_await） |
| `62_random.cpp` | 引擎与分布 |
| `63_charconv_streams.cpp` | charconv、stringstream、iomanip |
| `64_type_traits.cpp` | 类型萃取、typeid |
| `65_span_mdspan.cpp` | C++20 span、C++23 mdspan |
| `66_atomic.cpp` | atomic、内存序、atomic_ref |
| `67_sync_primitives.cpp` | C++20 semaphore/latch/barrier |
| `68_future.cpp` | async/promise/packaged_task |
| `69_pmr_allocators.cpp` | pmr 多态分配器、对齐 new |
| `70_compat_c_headers.cpp` | <cXXX> C 兼容头、宽字符 |
| `71_compare_misc.cpp` | C++20 <=>、source_location、valarray |

## 本机（Apple clang 21）尚不可用的标准库

- C：`<threads.h>`（用 pthread 替代，见 `c/25`）、`<stdbit.h>`（手写等价函数，见 `c/26`）
- C++：`std::generator`（手写协程替代，见 `cpp/61`）、`std::osyncstream`（锁保护输出替代，见 `cpp/60`）、部分 C++23 views（`enumerate/chunk/slide/stride`）

其余标准库头在本机均可编译运行。
