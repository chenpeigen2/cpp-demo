// 49 - <string>：std::string 与 string_view
#include <print>
#include <string>
#include <string_view>
#include <algorithm>

int main() {
    std::string s = "hello";
    s += ", world";                       // 拼接
    s.push_back('!');
    std::println("{} len={} cap~={}", s, s.size(), s.capacity());

    // 查找：返回下标，找不到是 npos
    std::println("find(world)={} rfind(o)={} 找不到={}",
                 s.find("world"), s.rfind('o'), s.find("xyz") == std::string::npos);

    // 子串、插入、替换、删除
    std::println("substr(7,5)={}", s.substr(7, 5));
    s.insert(5, " C++");                  // hello C++, world!
    s.replace(0, 5, "HELLO");
    s.erase(5, 4);
    std::println("改后: {}", s);

    // 与数字互转
    std::println("to_string={} stoi={} stod={}",
                 std::to_string(3.14), std::stoi("42abc"), std::stod("2.5e3"));

    // 比较：按字典序，== 直接比较内容
    std::println("abc<abd={} (==)={}", std::string("abc") < "abd", s == s);

    // C++23：string 的 contains / starts_with / ends_with
    std::println("contains('or')={} starts_with(HELLO)={} ends_with('!')={}",
                 s.contains("or"), s.starts_with("HELLO"), s.ends_with('!'));

    // string_view：不拥有数据的只读视图，零拷贝传参
    std::string_view sv = s;              // 指向 s，不分配内存
    std::string_view head = sv.substr(0, 5);  // substr 也是 O(1) 视图！
    std::println("sv={} head={}", sv, head);

    // 遍历
    int upper = 0;
    for (char c : sv) if (std::isupper((unsigned char)c)) upper++;
    std::println("大写字母数={}", upper);
    return 0;
}
