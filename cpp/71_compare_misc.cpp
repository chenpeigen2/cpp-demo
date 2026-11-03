// 71 - <compare> <source_location> <initializer_list> <valarray>：语言支持杂项
#include <print>
#include <compare>
#include <source_location>
#include <initializer_list>
#include <valarray>
#include <numeric>
#include <cmath>

// ---- <compare>：三路比较（"太空船"运算符 <=>，C++20）----
struct Version {
    int major, minor, patch;
    // 一次性生成 ==/!=/</>/<=/>= 全部六个比较
    auto operator<=>(const Version &) const = default;
};

// 自定义序：返回 strong_ordering / partial_ordering / weak_ordering
static std::strong_ordering cmp_int(int a, int b) { return a <=> b; }

// ---- <source_location>：编译期获取代码位置（替代 __LINE__/__FILE__ 宏）----
static void log_with_loc(std::string_view msg,
                         const std::source_location &loc = std::source_location::current()) {
    std::println("[{}:{}] {}", loc.file_name(), loc.line(), msg);
}

// ---- <initializer_list>：{} 初始化列表的幕后类型 ----
class Averages {
public:
    Averages(std::initializer_list<double> list) {
        double total = 0;
        for (double v : list) total += v;
        value_ = list.size() ? total / list.size() : 0;
    }
    double value() const { return value_; }
private:
    double value_ = 0;
};

int main() {
    // compare
    Version v1{1, 4, 2}, v2{1, 10, 0};
    std::println("1.4.2 < 1.10.0? {}（字典序比较每个成员）", v1 < v2);
    std::println("cmp(3,5) = {} 0 表示小于", cmp_int(3, 5) < 0);
    std::println("sp<=> 的值: {} / {} / {}",
                 (1 <=> 1) == std::strong_ordering::equal ? "相等" : "?",
                 (1 <=> 2) == std::strong_ordering::less ? "小于" : "?",
                 (2 <=> 1) == std::strong_ordering::greater ? "大于" : "?");

    // source_location
    log_with_loc("这行日志自带文件名和行号");

    // initializer_list
    Averages a{1.0, 2.0, 3.0, 4.0};
    std::println("initializer_list 平均值={}", a.value());

    // ---- <valarray>：面向数值计算的数组（自动逐元素运算）----
    std::valarray<double> va{1.0, 2.0, 3.0, 4.0};
    std::valarray<double> vb = va * 2.0 + 1.0;         // 逐元素：3 5 7 9
    std::println("valarray va*2+1 = {:.0f} {:.0f} {:.0f} {:.0f}",
                 vb[0], vb[1], vb[2], vb[3]);
    std::println("sum={:.1f} min={:.1f} max={:.1f}", vb.sum(), vb.min(), vb.max());

    // 切片：取子集运算
    std::slice s(0, 2, 2);                             // 从0开始，取2个，步长2
    std::valarray<double> picked = vb[s];
    std::println("切片(偶数位) = {:.0f} {:.0f}", picked[0], picked[1]);

    std::valarray<double> rt = std::sqrt(va);          // 逐元素数学函数
    std::println("sqrt 每个元素 = {:.2f} {:.2f} {:.2f} {:.2f}",
                 rt[0], rt[1], rt[2], rt[3]);
    std::println("(现代代码一般用 vector+ranges，valarray 是数值计算的历史遗珠)");
    return 0;
}
