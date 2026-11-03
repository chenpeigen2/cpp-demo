// 48 - <numeric> 补充与 <bit>：位操作（C++20 起标准库自带）
#include <print>
#include <numeric>
#include <bit>
#include <vector>
#include <cstdint>

int main() {
    // <bit>：编译器内置指令级的位操作
    std::uint32_t x = 0b1011;
    std::println("popcount={}", std::popcount(x));            // 1 的个数
    std::println("has_single_bit(8)={}", std::has_single_bit(8u));  // 是否 2 的幂
    std::println("bit_ceil(100)={}", std::bit_ceil(100u));    // 向上取 2 的幂
    std::println("bit_floor(100)={}", std::bit_floor(100u));  // 向下取 2 的幂
    std::println("countl_zero={}", std::countl_zero(x));      // 前导 0
    std::println("countr_one={}", std::countr_one(0b0111u));  // 尾部 1
    std::println("rotl(0b1000001,2)={}", std::rotl(0x41u, 2)); // 循环左移
    std::println("byteswap(0x12345678)={:x}", std::byteswap(0x12345678u)); // 字节序反转
    std::println("bit_width(100)={}", std::bit_width(100u));  // 有效位数
    std::println("endian: {}",
                 std::endian::native == std::endian::little ? "小端" : "大端");

    // <numeric> 剩余成员
    std::vector<int> v{1, 2, 3, 4};
    std::println("reduce={}", std::reduce(v.begin(), v.end()));   // 可并行的 accumulate
    std::println("transform_reduce={}",
                 std::transform_reduce(v.begin(), v.end(), 0, std::plus{}, [](int x) { return x * x; }));  // 平方和
    std::println("adjacent_difference:");
    std::vector<int> diff;
    std::adjacent_difference(v.begin(), v.end(), std::back_inserter(diff));
    std::println("adjacent_difference(相邻差): {}", diff);
    std::println("midpoint(3,8)={}", std::midpoint(3, 8));        // 防溢出的中点
    std::println("lerp(0,10,0.25)={:.1f}", std::lerp(0.0, 10.0, 0.25)); // 线性插值
    return 0;
}
