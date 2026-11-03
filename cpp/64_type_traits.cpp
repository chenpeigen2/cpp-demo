// 64 - <type_traits> <typeinfo>：编译期类型内省与运行期类型信息
#include <print>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <vector>

// type_traits 的典型用法：按类型特征选择实现
template <typename T>
std::string describe(T) {
    if constexpr (std::is_integral_v<T>)            // 编译期分支
        return "整数类型";
    else if constexpr (std::is_floating_point_v<T>)
        return "浮点类型";
    else if constexpr (std::is_pointer_v<T>)
        return "指针类型";
    else
        return "其他类型";
}

// enable_if / concepts 前身：SFINAE 时代的选择器（现代代码用 concepts）
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
T doubled(T x) { return x * 2; }

int main() {
    // 一元特征：is_xxx_v 直接给出编译期 bool
    std::println("is_integral<int>={} is_integral<double>={}",
                 std::is_integral_v<int>, std::is_integral_v<double>);
    std::println("is_pointer<int*>={} is_const<const int>={}",
                 std::is_pointer_v<int *>, std::is_const_v<const int>);
    std::println("is_same<int,int32_t>(LP64)={}", std::is_same_v<int, int32_t>);
    std::println("is_base_of: Animal:Dog 见 35");
    std::println("is_convertible<double,int>={} is_trivially_copyable<int>={}",
                 std::is_convertible_v<double, int>, std::is_trivially_copyable_v<int>);

    // 类型变换：remove/add/copy 限定符与引用
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::add_pointer_t<int>, int *>);
    static_assert(std::is_same_v<std::remove_reference_t<int &&>, int>);
    static_assert(std::is_same_v<std::decay_t<const int &>, int>);   // lambda 捕获分析常用
    std::println("类型变换全部通过 static_assert");

    // conditional：编译期三元运算符
    using Big = std::conditional_t<sizeof(void *) == 8, long long, int>;
    std::println("conditional 选择 long long: {}", sizeof(Big) == 8);

    // SFINAE 选择器
    int probe = 0;
    std::println("doubled(21)={} describe: {} {} {}",
                 doubled(21), describe(1), describe(1.5), describe(&probe));

    // ---- typeinfo：运行期类型识别 (RTTI) ----
    const std::type_info &ti = typeid(std::string);
    std::println("typeid(string).name()={} hash={}", ti.name(), ti.hash_code());
    std::println("typeid(42)==typeid(int)? {}", typeid(42) == typeid(int));

    // 多态类型的 typeid 看实际（动态）类型
    struct Base { virtual ~Base() = default; };
    struct Derived : Base {};
    Base *b = new Derived;
    std::println("b 的实际类型是 Derived? {}", typeid(*b) == typeid(Derived));
    delete b;

    // bad_cast 示意：引用型 dynamic_cast 失败时抛出（见 35 的指针版）
    return 0;
}
