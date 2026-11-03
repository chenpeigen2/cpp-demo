// 77 - 进阶：constexpr 计算与编译期数据 —— 把运行搬到编译
#include <print>
#include <array>
#include <ranges>
#include <chrono>

// constexpr 函数在编译期和运行期都能用（同一份代码两种身份）
constexpr long long fib(int n) {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);   // 编译期递归（-O2 下 30 层内秒出）
}

// C++23 起 constexpr 能力大扩张：大部分算法/容器操作都可在编译期执行
constexpr int sum_squares(const std::array<int, 5> &a) {
    int s = 0;
    for (int v : a) s += v * v;
    return s;
}

// 编译期生成的查找表：零运行时成本
constexpr std::array<long long, 20> FIB_TABLE = [] {
    std::array<long long, 20> t{};
    t[0] = 0; t[1] = 1;
    for (std::size_t i = 2; i < t.size(); i++) t[i] = t[i - 1] + t[i - 2];
    return t;
}();

// consteval：强制编译期（"立即函数"），运行期调用直接编译错误
consteval int compile_time_square(int x) { return x * x; }

// static_assert 就是编译期测试：TDD 的编译期形态
static_assert(FIB_TABLE[10] == 55);
static_assert(sum_squares({1, 2, 3, 4, 5}) == 55);
static_assert(compile_time_square(12) == 144);
static_assert(fib(20) == 6765);

// 编译期字符串处理（C++20 起 constexpr string 可用，26 再扩）
consteval std::size_t cstrlen(const char *s) {
    std::size_t n = 0;
    while (s[n]) n++;
    return n;
}
static_assert(cstrlen("hello") == 5);

int main() {
    std::println("编译期算的 fib(30) = {}", fib(30));
    std::println("查找表前 10 项: {}", FIB_TABLE | std::views::take(10));

    // 同一个函数运行期执行（对比耗时）
    auto t0 = std::chrono::steady_clock::now();
    volatile long long sink = 0;
    for (int i = 0; i < 30; i++) sink += fib(i);
    auto t1 = std::chrono::steady_clock::now();
    std::println("运行期再算 0..29: {:.0f}µs（编译期版本耗时 0）",
                 std::chrono::duration<double, std::micro>(t1 - t0).count());

    // consteval 只能编译期：
    std::println("consteval 编译期平方(12)={}", compile_time_square(12));
    // compile_time_square(runtime_val);   // ← 取消注释会编译错误：不是常量表达式

    // 编译期表 vs 运行期表：反汇编后编译期版本就是一串立即数
    constexpr auto ct = sum_squares({1, 2, 3, 4, 5});
    std::array<int, 5> rt{1, 2, 3, 4, 5};
    std::println("sum_squares: 编译期={} 运行期={}（结果相同，成本不同）",
                 ct, sum_squares(rt));
    std::println("原则：输入在编译期已知的计算，全部推到编译期 —— 程序变小、运行变快");
    return 0;
}
