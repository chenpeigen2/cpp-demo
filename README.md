# cpp-demo —— C23 / C++26 特性与标准库全览

用 **clang** 编译的 C 和 C++ 特性演示集。每个文件独立编译运行、单一主题、中文注释，可作为速查手册。

## 构建（macOS / Apple clang 21）

```sh
make        # 编译全部 102 个演示（C 用 -std=c2x 即 C23，C++ 用 -std=c++26）
make run    # 编译并依次运行，任何一个失败即报错
make clean
```

要求：Apple clang 21+（随 Xcode Command Line Tools 安装）。

## 目录

| 目录 | 内容 |
|---|---|
| `c/`  | C23 语言特性 + 标准库头巡礼（27 个） + 实战专题（11 个） |
| `cpp/` | C++26（向后覆盖 C++23/20）核心语言 + 标准库巡礼（44 个） + 进阶专题（20 个） |

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
| `27_embed.c` | C23 `#embed`：编译期嵌入文件 |

### C 实战专题（手写数据结构与硬核内幕，带实测数据）

| 文件 | 主题 |
|---|---|
| `28_vector.c` | 手写泛型动态数组（void*+elem，扩容数据） |
| `29_hashtable.c` | 手写哈希表（FNV-1a + 线性探测 + 碰撞统计） |
| `30_linkedlist.c` | 哨兵双向链表 + 侵入式链表（container_of） |
| `31_float_bits.c` | IEEE-754 拆位：0.1+0.2、累积误差、epsilon |
| `32_ub.c` | 未定义行为展览（10 项 UB + 正确做法表） |
| `33_endian_align.c` | 字节序、对齐、padding 布局数据 |
| `34_xmacro.c` | X-macro 元编程（枚举↔字符串、字段反射表） |
| `35_benchmark.c` | 微基准：memcpy vs 循环、strlen 缓存等 ns/op 表 |
| `36_memory_pool.c` | 内存池：arena bump + 固定块 free-list（分配快几十倍实测） |
| `37_state_machine.c` | 数据驱动状态机：转移表 + 函数指针动作 |
| `38_string_builder.c` | 倍增缓冲 vs strcat O(n²)（实测 10 倍+差距） |

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
| `72_cpp26_lang.cpp` | C++26：占位符 `_`、包索引、`=delete("原因")`、static operator() |
| `73_cpp26_lib.cpp` | C++26 库：饱和算术、span 列表初始化、ranges 增强 |

### C++ 进阶专题（惯用法、并发、性能实测数据）

| 文件 | 主题 |
|---|---|
| `74_crtp.cpp` | CRTP 静态多态 + 虚函数开销实测 |
| `75_type_erasure.cpp` | 类型擦除三法（手写 vtable / function / 模板） |
| `76_perfect_forwarding.cpp` | 完美转发：引用折叠、emplace 数据、拷贝/移动计数 |
| `77_constexpr.cpp` | 编译期计算：consteval、查找表、static_assert 测试 |
| `78_virtual_diamond.cpp` | 虚表/vbptr 内存数据 + 菱形继承 + 虚调用开销实测 |
| `79_strong_types.cpp` | 强类型与单位（米/秒/chrono，编译期拦截混用） |
| `80_pimpl.cpp` | Pimpl：编译防火墙、ABI、移动一个指针的移动语义 |
| `81_custom_iterator.cpp` | 手写环形缓冲 + 迭代器，static_assert 概念面试 |
| `82_thread_pool.cpp` | 手写线程池 + 加速比实测 + 异常穿透 |
| `83_memory_layout.cpp` | sizeof/alignof 数据表、EBO、variant vs union |
| `84_template_meta.cpp` | 模板元编程：类型表、惰性递归陷阱、TMP→concepts 演进 |
| `85_container_perf.cpp` | 容器性能：reserve/拷贝vs移动/vector vs list/字符串拼接 |
| `86_spaceship_custom.cpp` | <=> 定制：strong/weak/partial 三档相等强度 |
| `87_flat_map_perf.cpp` | flat_map vs map vs unordered_map 三方实测选型 |
| `88_rvo_elision.cpp` | RVO/拷贝消除：构造/拷贝/移动计数实测表 |
| `89_sso_string.cpp` | SSO 短字符串优化：栈/堆判定 + 性能差异实测 |
| `90_custom_view.cpp` | 手写 ranges 视图：迭代器 + 管道闭包 + view_interface |
| `91_ast_visit.cpp` | AST 求值：variant+visit vs 虚函数（表达式问题） |
| `92_allocators.cpp` | 自定义分配器统计：看见容器的每一次堆分配 |
| `93_concepts_advanced.cpp` | 概念进阶：蕴含、requires 五种形态、缩写模板 |

## 学习路径

**入门**（语言基础）：c/01→10，cpp/30→34
**进阶**（标准库全景）：c/14→18，cpp/40→59
**高手**（惯用法与性能）：cpp/74→93，c/28→38
**参考书定位**：遇到忘了的语法按 README 索引查；每文件末尾注释都给出'工程经验'结论

## 本机（Apple clang 21）尚不可用的标准库

- C：`<threads.h>`（用 pthread 替代，见 `c/25`）、`<stdbit.h>`（手写等价函数，见 `c/26`）
- C++：`std::generator`（手写协程替代，见 `cpp/61`）、`std::osyncstream`（锁保护输出替代，见 `cpp/60`）、部分 C++23/26 views（`enumerate/chunk/slide/stride`）、C++26 契约（clang 21 未实现 P2900）、`constexpr` placement new（P2747）

其余标准库头在本机均可编译运行。
