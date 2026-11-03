// 55 - <tuple> <utility> <pair>：异构组合与解包
#include <print>
#include <tuple>
#include <utility>
#include <map>
#include <string>

std::tuple<std::string, int, double> get_user() {   // 多返回值：比出参优雅
    return {"alice", 30, 55.5};
}

int main() {
    // tuple：任意类型、任意数量的组合
    auto [name, age, score] = get_user();           // 结构化绑定拆包
    std::println("name={} age={} score={}", name, age, score);

    // get 按下标/类型取；tie 按引用拆包忽略部分值
    auto t = std::make_tuple(1, 'x', 3.14);
    std::println("get<0>={} get<char>={}", std::get<0>(t), std::get<char>(t));
    int i; std::string s;
    std::tie(i, std::ignore, s) = std::make_tuple(42, 0.0, std::string("hi"));
    std::println("tie: i={} s={}", i, s);

    // tuple 比较：按字典序逐元素比较
    std::println("tuple 比较: {}", std::make_tuple(1, 2) < std::make_tuple(1, 3));

    // tuple_cat 拼接；apply 把 tuple 展开成函数参数
    auto merged = std::tuple_cat(t, std::make_tuple(true));
    std::println("拼接后大小={}", std::tuple_size_v<decltype(merged)>);
    std::println("apply(add,(2,3))={}", std::apply([](int a, int b) { return a + b; },
                                                    std::make_tuple(2, 3)));

    // pair：二元特例，map 的元素类型就是它
    std::pair<std::string, int> p{"key", 1};
    p.first = "renamed"; p.second++;
    std::println("pair: {} {}", p.first, p.second);

    std::map<std::string, int> m{{"a", 1}};
    auto [it, ok] = m.insert({"b", 2});             // map::insert 返回 pair
    std::println("insert b: {} 键={}", ok, it->first);

    // <utility> 常用工具
    int x = 5;
    int y = std::exchange(x, 99);                   // 返回旧值并赋新值
    std::println("exchange: 旧={} 新={}", y, x);
    const int &cx = std::as_const(x);               // 强制加 const 视图
    std::println("as_const={}", cx);
    std::println("forward 演示见 64; in_place 见 56");
    return 0;
}
