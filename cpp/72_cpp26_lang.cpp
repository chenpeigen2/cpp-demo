// 72 - C++26 语言特性（Apple clang 21 已实现的部分）
#include <print>
#include <utility>

// P1169 static operator()：不需要对象状态时可声明为静态（省 this 指针开销）
struct Next {
    static int operator()(int x) { return x + 1; }
    static void operator[](int) {}
};

// P2573 = delete("原因")：删除原因会出现在编译错误信息里
struct CopyableOnly {
    CopyableOnly() = default;
    CopyableOnly(const CopyableOnly &) = default;
    CopyableOnly(CopyableOnly &&) = delete("此类型按设计必须可拷贝，移动会破坏共享状态");
};

int main() {
    // P2169R1 占位符变量 _：可重复声明、重复赋值，语义上"丢弃"
    int _ = 1;                     // 每个 _ 都是独立的未命名变量
    _ = 2;                         // 不警告"未使用"，也不与上面的冲突
    auto [x, _2] = std::pair{1, 2};   // 结构化绑定里忽略不需要的成员
    std::println("占位符 _: x={}（_ 只表示'这里有个变量，但我不关心它'）", x);

    // P2662R3 包索引：按下标直接取参数包第 i 个类型/值，告别递归展开
    auto first_type_name = []<typename... Ts>(Ts... ts) {
        using First = Ts...[0];                     // 类型层面：第 0 个类型
        using Last = Ts...[sizeof...(Ts) - 1];      // 最后一个
        if constexpr (std::is_same_v<First, Last>)
            return ts...[0];                        // 值层面：取第 0 个值
        else
            return ts...[0];
    };
    std::println("包索引: 首元素={}", first_type_name(10, 2.5, 'c'));

    // static operator()：像普通函数一样调用，无需对象状态
    std::println("static operator(): Next{{}}(41)={}", Next{}(41));

    // = delete("原因") 的效果（取消注释编译会看到中文错误信息）：
    // CopyableOnly a; auto b = std::move(a);

    // C++26 模式下 C++23/26 的标准属性继续可用
    [[assume(x == 1)]];            // P1774 假设标注（优化提示）
    std::println("assume 标注通过，x 仍为 {}", x);
    return 0;
}
