// 46 - 范围 (ranges)：C++20/23 的现代序列操作
#include <print>
#include <ranges>
#include <vector>
#include <string>
#include <algorithm>

namespace rv = std::views;               // views 是"惰性视图"，不拷贝数据

int main() {
    std::vector<int> v{8, 3, 7, 1, 9, 4, 6};

    // 管道风格：filter | transform | take，全是惰性的（不产生中间容器）
    auto result = v | rv::filter([](int x) { return x % 2 == 0; })
                    | rv::transform([](int x) { return x * 10; });
    std::println("偶数x10: {}", result);

    // iota：生成 0,1,2,...（可无限），配合 take 截断
    std::println("iota: {}", rv::iota(1) | rv::take(5));

    // 带下标遍历：zip(iota, 容器) 组合两个视图
    for (auto [i, x] : rv::zip(rv::iota(0), v) | rv::take(3))
        std::print("[{}]{} ", i, x);
    std::println("");

    // C++23 views
    std::println("zip 两序列: {}", rv::zip(v, rv::iota(100)));   // 等长拉链
    std::println("adjacent 相邻对: {}", v | rv::adjacent<2>);    // (8,3),(3,7)...
    std::println("reverse 取前3: {}", v | rv::reverse | rv::take(3));
    std::println("drop(2) 取前3: {}", v | rv::drop(2) | rv::take(3));
    std::println("iota 字母: {}", rv::iota('a', 'f'));
    std::println("repeat+take: {}", rv::repeat(7) | rv::take(3)); // 无限重复值

    // 字符串切分：split view（C++23 起可直接输出）
    std::string csv = "a,b,c";
    for (auto &&word : csv | rv::split(','))
        std::print("[{}] ", std::string_view(word));
    std::println("");

    // 算法版：std::ranges::xxx 直接收容器，不必写 begin/end
    std::ranges::sort(v);
    std::println("ranges::sort: {}", v);
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);                    // unique 只搬动，真正删除要 erase
    std::println("去重后: {}", v);
    std::println("ranges::count(奇数)={}",
                 std::ranges::count_if(v, [](int x) { return x % 2; }));
    std::println("ranges::contains(4)={}", std::ranges::contains(v, 4));  // C++23

    // 投影 (projection)：算法直接按成员比较，不用写 lambda
    struct P { std::string name; int age; };
    std::vector<P> people{{"bob", 25}, {"alice", 30}};
    std::ranges::sort(people, {}, &P::age);                  // 按 age 排
    std::println("最年轻: {}", people[0].name);
    return 0;
}
