// 37 - 模板基础：函数模板与类模板
#include <print>
#include <vector>
#include <string>

template <typename T>                          // 函数模板：编译器按实参推导 T
T maximum(const T &a, const T &b) {
    return a > b ? a : b;
}

template <typename T>                          // 类模板
class Stack {
    std::vector<T> data_;
public:
    Stack() = default;
    Stack(std::initializer_list<T> init) : data_(init) {}  // 支持 Stack s{1,2,3}
    void push(const T &v) { data_.push_back(v); }
    T pop() { T v = data_.back(); data_.pop_back(); return v; }
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
};

// 非类型模板参数：值也是模板参数
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;  // 编译期递归
};
template <> struct Factorial<0> { static constexpr int value = 1; };  // 特化终止

int main() {
    std::println("max(3,9)={} max(1.5,0.5)={} max<std::string>={}",
                 maximum(3, 9), maximum(1.5, 0.5), maximum(std::string("abc"), std::string("abd")));

    Stack<int> s;
    s.push(1); s.push(2); s.push(3);
    std::println("stack: pop {} size {}", s.pop(), s.size());

    Stack names{std::string("a"), std::string("b")};   // CTAD
    std::println("names size={}", names.size());

    std::println("5! = {}", Factorial<5>::value);      // 编译期就算完了
    static_assert(Factorial<5>::value == 120);
    return 0;
}
