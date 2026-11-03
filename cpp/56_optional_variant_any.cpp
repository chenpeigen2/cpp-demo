// 56 - <optional> <variant> <any>：三种"可能不是值"的容器
#include <print>
#include <optional>
#include <variant>
#include <any>
#include <string>

// optional：可能有值可能没有（比哨兵值/-1/nullptr 语义清晰）
std::optional<int> parse_int(std::string_view s) {
    if (s.empty() || !std::isdigit((unsigned char)s[0])) return std::nullopt;
    int v = 0;
    for (char c : s) v = v * 10 + (c - '0');
    return v;
}

// variant：类型安全的"union"，同一时刻持有其中一种类型
using Config = std::variant<int, std::string, double>;

struct PrintVisitor {                     // 访问器：对每种可能类型重载
    void operator()(int v) const { std::println("配置是整数 {}", v); }
    void operator()(const std::string &v) const { std::println("配置是字符串 '{}'", v); }
    void operator()(double v) const { std::println("配置是浮点 {}", v); }
};

int main() {
    // optional 的用法
    if (auto v = parse_int("12345"))       // optional 可直接做布尔判断
        std::println("解析成功={}", *v);   // *v 解引用（确认有值后）
    auto bad = parse_int("abc");
    std::println("解析失败? has_value={} value_or={}", bad.has_value(), bad.value_or(-1));
    // bad.value() 会抛 bad_optional_access；value_or 给默认值更安全

    // variant 的用法
    Config c = 42;
    std::visit(PrintVisitor{}, c);
    c = std::string("hello");
    std::visit(PrintVisitor{}, c);

    // holds_alternative 检查类型；get 取值（类型错会抛 bad_variant_access）
    std::println("当前是 string? {}", std::holds_alternative<std::string>(c));
    std::println("index={}", c.index());
    c = 3.14;
    std::println("get_if(double)={:.2f}", *std::get_if<double>(&c));

    // variant + overload 惯用法：lambda 集合当访问器
    auto visitor = [](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) std::println("lambda 访问器: int {}", arg);
        else std::println("lambda 访问器: 其他");
    };
    std::visit(visitor, c);

    // any：完全动态的类型（运行期查类型，有代价，少用）
    std::any a = 1;
    a = std::string("now a string");
    try {
        std::println("any={}", std::any_cast<std::string>(a));
    } catch (const std::bad_any_cast &e) {
        std::println("转型失败: {}", e.what());
    }
    std::println("any 类型名={}", a.type().name());   // typeinfo 名字（实现定义）
    return 0;
}
