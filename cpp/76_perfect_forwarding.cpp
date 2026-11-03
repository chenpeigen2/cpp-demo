// 76 - 进阶：完美转发 —— 万能引用、引用折叠、转发引用矩阵
#include <print>
#include <utility>
#include <string>
#include <vector>

struct Widget {
    std::string name;
    Widget() : name("默认") { std::println("  [构造默认]"); }
    explicit Widget(std::string n) : name(std::move(n)) { std::println("  [构造 {}]", name); }
    Widget(const Widget &o) : name(o.name) { std::println("  [拷贝构造 {}]", name); }
    Widget(Widget &&o) noexcept : name(std::move(o.name)) { std::println("  [移动构造 {}]", name); }
    Widget &operator=(const Widget &) = default;
    Widget &operator=(Widget &&) = default;
};

// T&& 在模板推导中是"万能引用"：左值绑成 T&，右值绑成 T（引用折叠规则）
//   & + && = &     && + && = &&     （这就是 std::forward 能恢复值类别的原因）
template <typename T>
Widget make_widget(T &&arg) {
    // forward<T>：左值仍转发为左值，右值恢复为右值 —— 不转发就全变左值！
    return Widget(std::forward<T>(arg));
}

// 典型错误对照：不加 forward
template <typename T>
Widget make_widget_wrong(T &&arg) {
    return Widget(arg);                    // arg 是具名变量 → 永远按左值处理
}

// emplace 风格：参数包一路转发到底层构造
template <typename... Args>
Widget make_with_args(Args &&...args) {
    return Widget(std::forward<Args>(args)...);
}

static int copies = 0, moves = 0;

struct Tracked {
    Tracked() = default;
    Tracked(const Tracked &) { copies++; }
    Tracked(Tracked &&) { moves++; }
};

// 转发进容器的数据
static std::vector<Tracked> sink;
template <typename T>
static void store(T &&t) { sink.emplace_back(std::forward<T>(t)); }

int main() {
    Widget w{"手写"};
    std::string s = "字符串参数";

    std::println("① 转发右值字面量:");
    make_widget(std::string("临时"));       // 移动（字符串先构造，再移动进 Widget）
    std::println("② 转发左值 s:");
    make_widget(s);                          // 拷贝（源还在，必须拷）
    std::println("③ 错误写法转发右值:");
    make_widget_wrong(std::string("临时"));  // 变成拷贝！丢失移动机会
    std::println("④ std::move 后转发:");
    make_widget(std::move(w));               // 移动

    std::println("⑤ 参数包一路转发构造:");
    make_with_args(std::string{"emplace"});  // 直接原地构造，零多余拷贝

    // 用计数器验证转发正确性
    Tracked t;
    copies = moves = 0;
    store(t);                                // 期望：1 拷贝
    store(Tracked{});                        // 期望：1 移动（临时直接进容器）
    Tracked t2;
    store(std::move(t2));                    // 期望：1 移动
    std::println("\nemplace_back+forward 计数: 拷贝={} 移动={}（应为 1/2）", copies, moves);

    // 引用折叠速查（static_assert 编译期验证）
    using L = int &;
    using R = int &&;
    static_assert(std::is_same_v<L &&, int &>);      // & + && = &
    static_assert(std::is_same_v<R &&, int &&>);     // && + && = &&
    std::println("引用折叠规则已用 static_assert 验证：&+&&=&，&&+&&=&&");
    return 0;
}
