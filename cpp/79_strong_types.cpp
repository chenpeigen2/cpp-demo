// 79 - 进阶：强类型与物理单位 —— 让编译器拦住"米加秒"
#include <print>
#include <chrono>
#include <cstdint>
#include <map>

// ---- 手写强类型包装（BOOST_STRONG_TYPEDEF 的思路）----
template <typename Tag, typename Rep = double>
struct Strong {
    Rep value{};
    static constexpr std::string_view tag = Tag::name;
    constexpr explicit Strong(Rep v) : value(v) {}
    constexpr Rep raw() const { return value; }
    constexpr auto operator<=>(const Strong &) const = default;   // 可比较 → 能进 map/set
};
struct MeterTag   { static constexpr auto name = "米"; };
struct SecondTag  { static constexpr auto name = "秒"; };
struct OrderIdTag { static constexpr auto name = "order"; };
struct UserIdTag  { static constexpr auto name = "user"; };

using Meter  = Strong<MeterTag>;
using Second = Strong<SecondTag>;

// 加法在同类内合法；跨类相加无法通过编译（这正是目的）
constexpr Meter operator+(Meter a, Meter b) { return Meter{a.raw() + b.raw()}; }
constexpr Meter operator*(Meter a, double k) { return Meter{a.raw() * k}; }

// ---- chrono::duration 就是工业级强类型单位：数量 + 比率刻在类型里 ----
using namespace std::chrono_literals;
using namespace std::chrono;

// 速度类型：米/秒 —— duration_ratio 的 C++ 侧同款思路
struct Velocity {
    double m_per_s;
    static Velocity from(Meter m, Second s) { return {m.raw() / s.raw()}; }
};

int main() {
    Meter m1{100.0}, m2{23.5};
    Meter total = m1 + m2;
    std::println("米 + 米 = {} {}（强类型允许）", total.raw(), Meter::tag);

    Second sec{5.0};
    Velocity v = Velocity::from(total, sec);
    std::println("速度 = {} m/s", v.m_per_s);

    // Meter wrong = total + sec;   // ← 取消注释：两种 Strong 类型不匹配，编译错误
    // double d = total;            // ← explicit 阻止隐式转 double

    // chrono 的单位自动换算（整数比率刻在类型系统里，编译期完成）
    auto half_min = 500ms;
    duration<double, std::ratio<1>> secs = half_min;      // 毫秒→秒：无损，隐式允许
    std::println("500ms = {} s（duration 隐式无损转换）", secs.count());
    auto minutes_val = duration_cast<minutes>(2500ms);    // 有损：必须显式 cast
    std::println("2500ms → minutes（截断）= {}", minutes_val.count());

    // 常见 bug 被 chrono 拦住：sleep 参数单位混乱
    std::println("sleep_for(100ms) 与 sleep_for(milliseconds(100)) 同义，类型即文档");

    // 强类型 ID：不同表的 id 不能互串（防"把订单 id 当用户 id 查"）
    using OrderId = Strong<OrderIdTag, std::int64_t>;
    using UserId  = Strong<UserIdTag, std::int64_t>;
    (void)sizeof(UserId);   // 仅声明类型供注释里的反例使用
    std::map<OrderId, const char *> orders;
    orders[OrderId{1001}] = "已发货";
    // orders[UserId{1001}] = "?";   // ← 类型不同，编译错误
    std::println("OrderId 1001 = {}（UserId 1001 想查？编译器说不）",
                 orders[OrderId{1001}]);
    std::println("代价为零：Strong 全部 constexpr、无虚函数、无堆 —— 纯编译期约束");
    return 0;
}
