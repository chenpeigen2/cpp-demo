// 91 - 进阶：AST 求值 —— variant+visit vs 虚函数（表达式问题）
#include <print>
#include <variant>
#include <memory>
#include <string>

// ================= 写法 A：variant + visit（closed set of types）=================
struct Num { double value; };
struct Add { const struct Expr *lhs, *rhs; };
struct Mul { const struct Expr *lhs, *rhs; };

struct Expr {                       // 递归变体需要一层包装
    std::variant<Num, Add, Mul> node;
};

// overloaded 惯用法：lambda 集合变访问器（C++23 起可用 std::visit + 下标？仍是此惯用法）
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

double eval(const Expr *e) {
    return std::visit(overloaded{
        [](const Num &n) { return n.value; },
        [](const Add &a) { return eval(a.lhs) + eval(a.rhs); },
        [](const Mul &m) { return eval(m.lhs) * eval(m.rhs); },
    }, e->node);
}

std::string repr(const Expr *e) {
    return std::visit(overloaded{
        [](const Num &n) { return std::to_string(n.value); },
        [](const Add &a) { return "(" + repr(a.lhs) + " + " + repr(a.rhs) + ")"; },
        [](const Mul &m) { return "(" + repr(m.lhs) + " * " + repr(m.rhs) + ")"; },
    }, e->node);
}

// ================= 写法 B：虚函数（open set of types）=================
struct VExpr {
    virtual double eval() const = 0;
    virtual std::string repr() const = 0;
    virtual ~VExpr() = default;
};
struct VNum final : VExpr {
    double value;
    explicit VNum(double v) : value(v) {}
    double eval() const override { return value; }
    std::string repr() const override { return std::to_string(value); }
};
struct VBinOp final : VExpr {          // 二元运算共用一个类（+ 和 * 只差函数指针）
    char op;
    std::unique_ptr<VExpr> lhs, rhs;
    VBinOp(char o, std::unique_ptr<VExpr> l, std::unique_ptr<VExpr> r)
        : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
    double eval() const override { return op == '+' ? lhs->eval() + rhs->eval() : lhs->eval() * rhs->eval(); }
    std::string repr() const override {
        return "(" + lhs->repr() + " " + op + " " + rhs->repr() + ")";
    }
};

int main() {
    // 树：((1 + 2) * 3) + (4 * 10)
    using P = const Expr *;
    Expr one{Num{1}}, two{Num{2}}, three{Num{3}}, four{Num{4}}, ten{Num{10}};
    Expr add12{Add{P{&one}, P{&two}}};
    Expr mul123{Mul{P{&add12}, P{&three}}};
    Expr mul410{Mul{P{&four}, P{&ten}}};
    Expr root{Add{P{&mul123}, P{&mul410}}};

    std::println("variant 版: {} = {}", repr(&root), eval(&root));

    // 同一棵树，虚函数版
    auto tree = std::make_unique<VBinOp>('+',
        std::make_unique<VBinOp>('*',
            std::make_unique<VBinOp>('+', std::make_unique<VNum>(1), std::make_unique<VNum>(2)),
            std::make_unique<VNum>(3)),
        std::make_unique<VBinOp>('*', std::make_unique<VNum>(4), std::make_unique<VNum>(10)));
    std::println("虚函数版:   {} = {}", tree->repr(), tree->eval());

    // 两篇经典论文的取舍（"表达式问题"）：
    std::println("\n加新【操作】（如 optimize()）：");
    std::println("  variant: 写一个新 visit —— 类型定义完全不动 ✔");
    std::println("  虚函数:  每个子类都要加一个 override，改全部头文件 ✘");
    std::println("加新【类型】（如 Neg 一元负号）：");
    std::println("  variant: 所有 visit 的 overloaded 都要加分支，漏了编译期报错 ✔/✘");
    std::println("  虚函数:  新增一个子类即可，已有代码零改动 ✔");
    std::println("选型：操作集合稳定、类型频繁变 → 虚函数；类型固定、操作频繁变 → variant+visit");
    std::println("（编译器/解释器对固定 AST 跑大量 pass —— 这就是它们偏爱 variant 的原因）");
    return 0;
}
