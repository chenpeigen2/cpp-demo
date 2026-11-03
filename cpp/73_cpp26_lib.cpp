// 73 - C++26 标准库新增（Apple clang 21 / libc++ 已实现的部分）
#include <print>
#include <numeric>
#include <span>
#include <ranges>
#include <vector>
#include <climits>

int main() {
    // P0543 饱和算术（<numeric>）：溢出不回绕，而是钳制在边界
    std::println("add_sat(INT_MAX, 1) = {}", std::add_sat(INT_MAX, 1));      // 不溢出
    std::println("sub_sat(INT_MIN, 1) = {}", std::sub_sat(INT_MIN, 1));      // 钳在最小
    std::println("mul_sat(INT_MAX, 2) = {}", std::mul_sat(INT_MAX, 2));
    std::println("div_sat(-100, 0.0)? 只支持整数：div_sat(INT_MIN, -1) = {}",
                 std::div_sat(INT_MIN, -1));                                  // 防除法溢出

    // P2447 span 支持花括号初始化列表（只读 span）
    std::span<const int> s{1, 2, 3, 4};     // 不用先建 vector 了
    std::println("span 列表初始化: {}", s);

    // ranges 持续增强（__cpp_lib_ranges = 202406）：C++23/26 视图
    std::vector<int> v{5, 1, 4, 1, 3};
    std::println("adjacent 相邻差: {}", v | std::views::adjacent<2>
                 | std::views::transform([](auto pair) {
                       return std::get<1>(pair) - std::get<0>(pair); }));
    std::println("zip 拉链: {}", std::views::zip(v, std::views::iota(1)));

    // C++23 已有、26 继续巩固的 ranges::to（视图物化为容器）
    auto squared = v | std::views::filter([](int x) { return x % 2; })
                     | std::ranges::to<std::vector>();
    std::println("filter+to<vector>: {}", squared);
    return 0;
}
