// 84 - 进阶：模板元编程 —— 类型即计算（C++ TMP 的编译期函数库）
#include <print>
#include <type_traits>
#include <array>
#include <ranges>

// ---- 类型级"链表"：Cons 单元 ----
struct Nil {};
template <typename Head, typename Tail>
struct Cons {};

using Ints = Cons<int, Cons<double, Cons<char, Nil>>>;

// Length：递归特化
template <typename L> struct Length;
template <> struct Length<Nil> { static constexpr std::size_t value = 0; };
template <typename H, typename T>
struct Length<Cons<H, T>> { static constexpr std::size_t value = 1 + Length<T>::value; };

// Contains：类型查找
template <typename L, typename X> struct Contains;
template <typename X> struct Contains<Nil, X> { static constexpr bool value = false; };
template <typename H, typename T, typename X>
struct Contains<Cons<H, T>, X> {
    static constexpr bool value = std::is_same_v<H, X> || Contains<T, X>::value;
};

// Map：对每个元素套一个变换（这里是加 const）
template <typename L> struct AddConst;
template <> struct AddConst<Nil> { using type = Nil; };
template <typename H, typename T>
struct AddConst<Cons<H, T>> { using type = Cons<std::add_const_t<H>, typename AddConst<T>::type>; };

// 编译期测试
static_assert(Length<Ints>::value == 3);
static_assert(Contains<Ints, double>::value);
static_assert(!Contains<Ints, float>::value);
static_assert(std::is_same_v<AddConst<Ints>::type, Cons<const int, Cons<const double, Cons<const char, Nil>>>>);

// ---- C++11/14 老派 TMP vs C++20 constexpr：同一算法两种写法 ----
constexpr bool is_prime(int n) {                    // constexpr 版：可读性完胜
    if (n < 2) return false;
    for (int d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

// 递归模板版：必须用"特化选择器"实现惰性终止——
// 三目运算符的两个分支都会被实例化，写成 (cond ? true : IsPrime<N,D+1>) 会无限递归！
template <int N, int D = 2, bool Done = (D * D > N)>
struct IsPrime;
template <int N, int D>
struct IsPrime<N, D, true> : std::true_type {};         // 试除超过 √N → 质数
template <int N, int D>
struct IsPrime<N, D, false> {
    static constexpr bool value = (N % D != 0) && IsPrime<N, D + 1>::value;
};
static_assert(IsPrime<97>::value);
static_assert(!IsPrime<91>::value);                  // 91 = 7×13
static_assert(is_prime(97) && !is_prime(91));        // 两种写法结论一致

// 编译期质数表：constexpr 版生成 + static_assert 抽查
constexpr auto primes_below_50 = [] {
    std::array<int, 15> out{};
    int n = 0;
    for (int i = 2; i < 50; i++)
        if (is_prime(i) && n < 15) out[n++] = i;
    return out;
}();
static_assert(primes_below_50[0] == 2 && primes_below_50[14] == 47);

// ---- 检测惯用法（SFINAE → 概念的前身）----
template <typename T, typename = void>
struct HasSize : std::false_type {};
template <typename T>
struct HasSize<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

static_assert(HasSize<std::array<int, 3>>::value);
static_assert(!HasSize<int>::value);

// 同样的判断用 concepts 表达（现代写法）
template <typename T>
concept HasSizeC = requires(T t) { t.size(); };
static_assert(HasSizeC<std::array<int, 3>>);

int main() {
    std::println("类型表长度 = {}，含 double? {}，含 float? {}",
                 Length<Ints>::value, Contains<Ints, double>::value, Contains<Ints, float>::value);
    std::print("50 以内质数(编译期生成): ");
    for (int p : primes_below_50 | std::ranges::views::take(15))
        if (p) std::print("{} ", p);
    std::println("");
    std::println("演进路线：TMP(特化递归) → SFINAE(void_t 检测) → concepts(requires 表达式)");
    std::println("现代准则：能用 constexpr 函数就别写 TMP；TMP 只剩在类型层面做变换的价值");
    return 0;
}
