// 54 - <functional>：函数对象、bind、引用包装、哈希
#include <print>
#include <functional>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>

int add(int a, int b) { return a + b; }

struct Mul {                              // 函数对象：重载 operator() 的类型
    int operator()(int a, int b) const { return a * b; }
};

int main() {
    // std::function：能装下任何"签名匹配的可调用物"（类型擦除）
    std::function<int(int, int)> f = add;             // 普通函数
    std::println("函数: {}", f(2, 3));
    f = [](int a, int b) { return a + b + 1; };       // lambda
    std::println("lambda: {}", f(2, 3));
    f = Mul{};                                        // 函数对象
    std::println("函数对象: {}", f(2, 3));

    // lambda 完整形态：捕获、参数、返回、mutable、泛型
    int base = 100;
    auto adder = [base](int x) mutable { return base += x; };    // 按值捕获可改副本
    std::println("值捕获: {} {}（base 不变 {}）", adder(1), adder(2), base);
    auto ref_adder = [&base](int x) { base += x; return base; }; // 按引用捕获改原值
    std::println("引用捕获: {} base={}", ref_adder(10), base);
    auto generic = [](auto a, auto b) { return a + b; };         // 泛型 lambda (C++14)
    std::println("泛型: {} {}", generic(1, 2), generic(std::string("a"), std::string("b")));

    // C++14 "初始化捕获"：把 move-only 对象搬进 lambda
    auto owned = [ptr = std::make_unique<int>(42)] { return *ptr; };
    std::println("捕获 unique_ptr={}", owned());

    // C++20 模板 lambda：显式模板参数
    auto sized = []<typename T>(const std::vector<T> &v) { return v.size(); };
    std::println("模板 lambda: {}", sized(std::vector<int>{1, 2, 3}));

    // bind：预绑定参数（现代代码多用 lambda，这里展示语法）
    auto add5 = std::bind(add, std::placeholders::_1, 5);
    std::println("bind: {}", add5(10));

    // 引用包装：把引用语义传进模板（std::thread、std::bind 等）
    int counter = 0;
    auto inc = std::ref(counter);
    inc.get()++;
    std::println("ref: counter={}", counter);

    // 标准函数对象：plus/less/greater 等
    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end(), std::greater<>{});   // 降序
    std::println("greater 排序: {}", v);
    std::println("plus<int>{}({},{})={}", "", 2, 3, std::plus<int>{}(2, 3));

    // std::hash：类型 → size_t，无序容器的地基
    std::println("hash(\"hi\") mod={} hash(42)={}",
                 std::hash<std::string>{}("hi") % 100000, std::hash<int>{}(42));
    return 0;
}
