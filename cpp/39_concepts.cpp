// 39 - 概念 (concepts) 与约束：C++20 给模板加上"类型要求说明书"
#include <print>
#include <concepts>
#include <string>
#include <vector>

// 标准概念：<integral> <floating_point> <same_as> <convertible_to> ...
template <std::integral T>
T twice(T x) { return x * 2; }             // 只接受整型

template <std::floating_point T>
T half(T x) { return x / 2; }

// 自定义概念：约束表达式，可组合（&& ||）
template <typename T>
concept Addable = requires(T a, T b) {     // requires 表达式：检查"能不能这么用"
    a + b;                                 // 要能相加
    { a + b } -> std::convertible_to<T>;   // 结果要能转回 T
};

template <Addable T>
T add(T a, T b) { return a + b; }

// requires 子句放在模板后：更复杂的条件
template <typename T>
    requires std::copyable<T> && std::equality_comparable<T>
bool contains(const std::vector<T> &v, const T &x) {
    for (const auto &e : v) if (e == x) return true;
    return false;
}

int main() {
    std::println("twice(21)={} half(3.0)={}", twice(21), half(3.0));
    std::println("add(1,2)={} add(std::string)={}", add(1, 2), add(std::string("ab"), std::string("cd")));

    std::vector<int> v{1, 2, 3};
    std::println("contains 2? {}", contains(v, 2));

    // 概念用于编译期分派（替代 tag dispatch / SFINAE）
    std::println("分类: {}", []<typename T>(T) {
        if constexpr (std::integral<T>) return "整型";
        else if constexpr (std::floating_point<T>) return "浮点";
        else return "其他";
    }(3.14));
    return 0;
}
