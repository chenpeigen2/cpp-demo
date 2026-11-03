// 38 - 模板进阶：特化、变参模板、折叠表达式
#include <print>
#include <string>
#include <utility>

// 全特化：为特定类型提供专门实现
template <typename T> struct TypeName { static constexpr char value[] = "unknown"; };
template <> struct TypeName<int>         { static constexpr char value[] = "int"; };
template <> struct TypeName<double>      { static constexpr char value[] = "double"; };

// 变参模板：接受任意数量、任意类型的参数
template <typename... Args>                // 参数包
void print_all(const Args &...args) {      // C++17 折叠表达式展开
    ((std::print("{} ", args), ...));      // 对每个 arg 执行 print，逗号连接
    std::println("");
}

// sizeof...(Args) 报参数个数；递归展开是 C++14 之前的老写法
template <typename T, typename... Rest>
constexpr auto sum(T first, Rest... rest) {
    if constexpr (sizeof...(rest) == 0)    // if constexpr：编译期分支，不满足的分支不实例化
        return first;
    else
        return first + sum(rest...);       // 递归消解参数包
}

int main() {
    std::println("{} {}", TypeName<int>::value, TypeName<double>::value);

    print_all(1, 2.5, "hello", 'x');
    print_all("只有字符串");
    std::println("sum(1,2,3,4)={} sum(0.5,0.25)={}", sum(1, 2, 3, 4), sum(0.5, 0.25));

    // std::index_sequence：编译期整数序列，常用于按序展开参数包
    constexpr auto seq = std::make_index_sequence<4>{};
    auto len = []<std::size_t... Is>(std::index_sequence<Is...>) {
        return sizeof...(Is);              // 参数包展开：sizeof... 数出 4 个
    };
    std::println("index_sequence 长度={}", len(seq));
    return 0;
}
