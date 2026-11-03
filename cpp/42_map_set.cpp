// 42 - 关联容器：map / set（红黑树，有序）
#include <print>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>

int main() {
    // map：键→值，按键有序，查找 O(log n)
    std::map<std::string, int> ages{{"alice", 30}, {"bob", 25}};
    ages["carol"] = 28;                      // 不存在则插入
    ages["alice"] = 31;                      // 存在则覆盖
    int n = ages["dave"];                    // 陷阱：[] 会插入默认值 0！
    std::println("ages: {}", ages);
    std::println("dave={}(被[]创建) count(alice)={}", n, ages.count("alice"));

    // 查找不插入：find / contains(C++20)
    if (auto it = ages.find("bob"); it != ages.end())
        std::println("find bob={} it->first={}", it->second, it->first);
    std::println("contains carol={}", ages.contains("carol"));

    // 遍历天然按 key 升序
    for (const auto &[name, age] : ages)     // 结构化绑定
        std::print("{}={} ", name, age);
    std::println("");

    // insert_or_assign / emplace：精确控制插入语义
    ages.insert_or_assign("eve", 40);
    ages.emplace("frank", 22);
    std::println("size={}", ages.size());

    // erase 按键删除；erase_if 批量条件删除(C++20)
    ages.erase("dave");
    std::erase_if(ages, [](const auto &p) { return p.second < 26; });
    std::println("删除后: {}", ages);

    // multimap：允许重复键（equal_range 取出一段）
    std::multimap<std::string, int> mm{{"a", 1}, {"a", 2}, {"b", 3}};
    std::println("multimap a 有 {} 个", mm.count("a"));

    // set：只存键的有序集合
    std::set<int> s{3, 1, 4, 1, 5, 9, 2, 6, 5};  // 重复自动去重
    std::println("set: {}", s);
    std::set<int> evens;
    std::copy_if(s.begin(), s.end(), std::inserter(evens, evens.end()),
                 [](int v) { return v % 2 == 0; });
    std::println("set 中的偶数: {}", evens);

    // multiset：可重复的有序集合
    std::multiset<int> ms{1, 1, 2};
    std::println("multiset count(1)={}", ms.count(1));
    return 0;
}
