// 57 - <expected> (C++23)：错误处理新范式，错误码与异常之间
#include <print>
#include <expected>
#include <string>
#include <cmath>

enum class ParseError { Empty, NotADigit };

// 返回"成功值或错误"，调用方被迫处理两种情况 —— 类型即文档
std::expected<int, ParseError> parse_digit(char c) {
    if (c < '0' || c > '9')
        return std::unexpected(ParseError::NotADigit);   // 错误路径
    return c - '0';                                      // 成功路径
}

// monadic 链式调用：and_then（成功才继续）/ or_else（失败才处理）/ transform
std::expected<int, ParseError> chain(char c) {
    return parse_digit(c)
        .and_then([](int v) -> std::expected<int, ParseError> {
            return v * 10;                               // 放大十倍
        })
        .transform([](int v) { return v + 1; });         // 成功后再变换
}

std::expected<double, std::string> safe_sqrt(double x) {
    if (x < 0) return std::unexpected("负数不能开方");
    return std::sqrt(x);
}

int main() {
    auto ok = parse_digit('7');
    auto bad = parse_digit('x');
    std::println("parse('7'): has_value={} 值={}", ok.has_value(), ok.value());
    std::println("parse('x'): 错误={}", (int)bad.error());

    // 链式
    auto r = chain('3');
    std::println("chain('3') = {}", r.value_or(-1));     // 3*10+1=31
    auto r2 = chain('z');
    std::println("chain('z') = {}（错误传播）", r2.value_or(-1));

    // 与异常对比：错误是返回值的一部分，函数签名一目了然
    if (auto s = safe_sqrt(2.0))
        std::println("sqrt(2)={:.4f}", *s);
    if (auto s = safe_sqrt(-1.0); !s)
        std::println("sqrt(-1) 出错: {}", s.error());

    // expected 的比较与解引用
    std::expected<int, ParseError> e1 = 5, e2 = 5;
    std::println("expected 相等? {}", e1 == e2);
    return 0;
}
