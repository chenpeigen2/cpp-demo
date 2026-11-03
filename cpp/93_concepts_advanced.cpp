// 93 - 进阶：概念进阶 —— 蕴含(subsumption)、requires 五种形态、缩写模板
#include <print>
#include <concepts>
#include <ranges>
#include <vector>
#include <string>
#include <list>

// ---- 1) 概念蕴含：更具体的概念自动胜出，无需标签分发 ----
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
template <typename T>
concept SignedNumeric = Numeric<T> && std::is_signed_v<T>;   // 蕴含 Numeric

int classify(Numeric auto)        { return 1; }   // 通用版
int classify(SignedNumeric auto)  { return 2; }   // 更受限版：编译器知道它更具体

// ---- 2) requires 表达式的完整形态清单 ----
template <typename T>
concept ShapeLike = requires(T t, const T ct, std::size_t i) {
    // ① 类型要求：必须有这些嵌套类型
    typename T::value_type;
    typename T::reference;

    // ② 简单要求：表达式必须合法
    t.size();
    t.clear();

    // ③ 复合要求：约束表达式类型（并可要求 noexcept）
    { t.size() } -> std::convertible_to<std::size_t>;
    { t[i] } noexcept -> std::convertible_to<typename T::reference>;

    // ④ 嵌套要求：追加另一个约束（常用于"先检查 T 满足 X"）
    requires std::movable<T>;
    requires std::ranges::sized_range<T>;
};

// 测试两个概念的真实类型
static_assert(ShapeLike<std::vector<int>>);
static_assert(ShapeLike<std::string>);       // string 其实满足全部要求
static_assert(!ShapeLike<std::list<int>>);   // list 没有 operator[]
static_assert(!ShapeLike<int>);

// ---- 3) requires 子句的四种位置/写法 ----
template <typename T>
    requires ShapeLike<T>                       // A：requires 子句
auto size_of(const T &t) { return t.size(); }

template <ShapeLike T>                          // B：概念直接当类型参数
auto size_of2(const T &t) { return t.size(); }

auto size_of3(ShapeLike auto const &t) { return t.size(); }   // C：缩写函数模板(C++20)

// D：函数体内的 if constexpr 分派（见 64 号）

// ---- 4) 概念不是 typedef：同一类型多次检查有不同"证据"，但蕴含可推导 ----
template <typename T>
concept Big = sizeof(T) >= 4;
template <typename T>
concept NumericAndBig = Numeric<T> && Big<T>;
static_assert(NumericAndBig<double>);            // 两个概念合取
static_assert(!NumericAndBig<char>);

int main() {
    // 蕴含实测：无符号走通用版，有符号走受限版 —— 没写任何 tag dispatch！
    std::println("classify(3u)   = {}（Numeric 版）", classify(3u));
    std::println("classify(-3)   = {}（SignedNumeric 版，更受约束者胜出）", classify(-3));
    std::println("classify(3.14) = {}（浮点不是 SignedNumeric？不：signed 也含浮点）", classify(3.14));

    // requires 表达式判定
    std::println("\nShapeLike: vector={} string={} list={} int={}",
                 ShapeLike<std::vector<int>>, ShapeLike<std::string>,
                 ShapeLike<std::list<int>>, ShapeLike<int>);

    // 三种调用形态等价
    std::vector<int> v{1, 2, 3};
    std::println("三种写法同一结果: {} {} {}",
                 size_of(v), size_of2(v), size_of3(v));

    // 缩写模板的 auto 也是概念（implicit standard concept）
    std::println("auto 参数也是约束: 无约束 auto 就是最弱的 constrained parameter");

    std::println("\n要点总结：");
    std::println("  · 蕴含自动决定重载选择，删掉 90% 的 tag_dispatch/SFINAE 代码");
    std::println("  · requires 表达式只查'能不能写出来'，不查语义 —— 概念名要表意(size() 不保证 O(1))");
    std::println("  · 错误信息对比：不满足概念 → 一行'do not satisfy'；SFINAE 失败 → 模板地狱");
    return 0;
}
