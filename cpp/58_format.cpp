// 58 - <format>：C++20 格式化 + C++23 <print>（printf 的现代替代）
#include <print>
#include <format>
#include <string>
#include <chrono>

struct Point {                            // 让自定义类型支持 std::format
    int x, y;
};

// 特化 formatter：解析格式串 + 输出
template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context &ctx) {   // 解析 {} 里的选项
        return ctx.begin();
    }
    auto format(const Point &p, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

int main() {
    // 基础占位：{} 自动编号，{0}{1} 手动编号
    std::println("{} + {} = {}", 1, 2, 3);
    std::println("{1} {0} {1}", "A", "B");

    // 对齐与填充：< 左对齐 > 右对齐 ^ 居中；数字前的数字是最小宽度
    std::println("[{:<8}] [{:>8}] [{:^8}] [{:*^8}]", "left", "right", "mid", "fill");
    std::println("整数填充: [{:05d}]", 42);

    // 类型指示符：d 十进制 x 十六进制 o 八进制 b 二进制 f 定点 e 科学计数 g 自动
    std::println("255 = {:d} {:x} {:o} {:b} {:#x}", 255, 255, 255, 255, 255);
    std::println("浮点: {:.2f} {:.0e} {:g}", 3.14159, 12345.6, 0.0001);

    // format 函数：返回 string 而不是打印
    std::string s = std::format("x={:>4}, y={:#06x}", 7, 255);
    std::println("format 返回: '{}'", s);

    // std::format_to 写入迭代器；std::formatted_size 算长度
    std::string buf;
    std::format_to(std::back_inserter(buf), "{}-{}", "a", 1);
    std::println("format_to: {} 长度={}", buf, std::formatted_size("{}-{}", "a", 1));

    // 编译期检查：format_string 在编译时验证参数（错误在编译期报出）
    std::println("chrono 格式化（C++20）: {}", std::chrono::weekday{std::chrono::Monday});

    // print 到指定流
    std::print(stderr, "(stderr via print)\n");
    return 0;
}
