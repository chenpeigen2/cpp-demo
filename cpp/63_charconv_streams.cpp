// 63 - <charconv> <sstream> <iomanip>：字符串与数字互转、流格式化
#include <print>
#include <iostream>
#include <charconv>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

int main() {
    // ---- charconv：无内存分配、无 locale、最快的数字↔字符串 ----
    char buf[16];
    auto [pend, ec] = std::to_chars(buf, buf + sizeof buf, 255, 16);  // 转十六进制
    *pend = '\0';
    std::println("to_chars(255,16)={} 错误={}", buf, ec == std::errc());

    const char *s = "42abc";
    int v = 0;
    auto [p2, ec2] = std::from_chars(s, s + 5, v);   // 解析整数，报出停在哪
    std::println("from_chars('42abc')={} 消耗了 {} 个字符", v, p2 - s);

#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    double d = 0;
    std::from_chars("3.14159", "3.14159" + 7, d);
    std::println("from_chars 浮点={:.4f}", d);
#else
    std::println("(libc++ 暂无浮点 from_chars)");
#endif

    // ---- stringstream：内存中的"流"，解析与拼接的经典工具 ----
    std::istringstream in("alice 30 55.5");
    std::string name;
    int age;
    double score;
    in >> name >> age >> score;                // 像 cin 一样解析
    std::println("istringstream 解析: {} {} {}", name, age, score);

    std::ostringstream out;
    out << "name=" << name << " age=" << age;
    std::println("ostringstream 拼接: {}", out.str());

    // 按空格切分（stringstream 当分词器用）
    std::istringstream words("one two three four");
    std::vector<std::string> parts;
    for (std::string w; words >> w;) parts.push_back(w);
    std::println("切分: {}", parts);

    // 字符串流与其他流的转换
    std::stringstream both;
    both << 123;
    int back = 0;
    both >> back;
    std::println("stringstream 往返: {}", back);

    // ---- iomanip：流格式化操纵器（iostream 的 format 选项）----
    std::println("--- iomanip ---");
    std::cout << std::setw(8) << 42 << "|\n";              // 宽度（只影响下一次输出）
    std::cout << std::setfill('*') << std::setw(8) << 42 << "|\n";
    std::cout << std::hex << std::showbase << 255 << std::dec << '\n';   // 0xff
    std::cout << std::fixed << std::setprecision(2) << 3.14159 << '\n';  // 3.14
    std::cout << std::boolalpha << true << '\n';           // true 而非 1
    std::println("(iomanip 基于 locale 的格式化已被 std::format 取代，此处演示语法)");
    return 0;
}
