// 50 - <regex>：正则表达式
#include <print>
#include <regex>
#include <string>
#include <iostream>

int main() {
    std::string text = "电话 13812345678，邮箱 alice@example.com，另一部 83912345678";

    // 匹配：regex_match 要求整个字符串匹配
    std::println("整串匹配 138...={}",
                 std::regex_match("13812345678", std::regex("1\\d{10}")));

    // 搜索：regex_search 在任意位置找
    std::regex phone_re(R"((1[3-9]\d{9}))");    // 手机号：1 开头、第二位 3-9、共 11 位
    if (std::smatch m; std::regex_search(text, m, phone_re)) {
        std::println("首个电话={} 前缀={}", m[0].str(), m[1].str());
    }

    // 迭代所有匹配
    std::print("全部电话: ");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), phone_re);
         it != std::sregex_iterator(); ++it)
        std::print("{} ", it->str());
    std::println("");

    // 替换
    std::string masked = std::regex_replace(text, phone_re, "$1****xxx");
    std::println("脱敏后: {}", masked);

    // 分组捕获 + 命名规则：解析 key=value
    std::regex kv(R"((\w+)=(\w+))");
    std::string conf = "host=localhost port=8080 debug=on";
    for (auto it = std::sregex_iterator(conf.begin(), conf.end(), kv);
         it != std::sregex_iterator(); ++it)
        std::println("  键={} 值={} ({})", (*it)[1].str(), (*it)[2].str(), (*it).position());

    // 常用语法速查：. 任意字符  \d 数字  \w 单词字符  + 1+  * 0+  ? 可选
    //               [] 字符集  ^ $ 锚点  {n,m} 重复次数  (|) 或/分组
    std::regex date_re(R"(\d{4}-\d{2}-\d{2})");
    std::println("日期匹配={}", std::regex_search("今天 2025-11-03 出发", date_re));
    return 0;
}
