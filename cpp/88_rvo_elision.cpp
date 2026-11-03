// 88 - 进阶：RVO 与拷贝消除 —— 返回值的真实成本（构造/拷贝/移动计数实测）
#include <print>

struct Trace {
    static inline int ctor = 0, copy = 0, move = 0, dtor = 0;
    int id;
    explicit Trace(int i) : id(i) { ctor++; }
    Trace(const Trace &o) : id(o.id) { copy++; }
    Trace(Trace &&o) noexcept : id(o.id) { move++; }
    ~Trace() { dtor++; }
};

// 1) 返回 prvalue（纯临时值）：C++17 起【保证】零拷贝 —— 不存在被移动的副本
Trace make_prvalue(int id) { return Trace{id}; }

// 2) 返回具名局部变量：NRVO（编译器把 t 直接构造在返回槽里）
Trace make_nrvo(int id) {
    Trace t{id};
    return t;                 // clang -O2 下 NRVO 生效：1 构造 0 移动
}

// 3) 多返回路径：NRVO 失败 → 退化为移动（不是拷贝！）
Trace make_branch(int id, bool alt) {
    Trace a{id}, b{id + 100};
    return alt ? a : b;       // 两个候选 → 至少一个被移动
}

// 4) std::move(return 局部) 是反模式：阻止 NRVO 反而多一次移动（故意触发 -Wpessimizing-move）
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpessimizing-move"
Trace make_pessimized(int id) {
    Trace t{id};
    return std::move(t);      // 编译器警告：move 会禁用 NRVO
}
#pragma clang diagnostic pop

// 5) sink 参数：按值拿 + move 进成员（现代 C++ 的标准写法）
struct Holder {
    Trace t;
    explicit Holder(Trace v) : t(std::move(v)) {}   // 调用方决定拷贝还是移动
};

int main() {
    std::println("场景                          构造 拷贝 移动");
    Trace::ctor = Trace::copy = Trace::move = 0;
    {
        Trace t = make_prvalue(1);
    }
    std::println("① 返回 prvalue                {:>3} {:>4} {:>4}  ← C++17 保证零拷贝",
                 Trace::ctor, Trace::copy, Trace::move);

    Trace::ctor = Trace::copy = Trace::move = 0;
    {
        Trace t = make_nrvo(2);
    }
    std::println("② 返回具名变量(NRVO)          {:>3} {:>4} {:>4}  ← 编译器直接在返回槽构造",
                 Trace::ctor, Trace::copy, Trace::move);

    Trace::ctor = Trace::copy = Trace::move = 0;
    {
        Trace t = make_branch(3, true);
    }
    std::println("③ 多路径返回(NRVO 失败)       {:>3} {:>4} {:>4}  ← 退化为移动，仍无拷贝",
                 Trace::ctor, Trace::copy, Trace::move);

    Trace::ctor = Trace::copy = Trace::move = 0;
    {
        Trace t = make_pessimized(4);
    }
    std::println("④ return std::move(t) 反模式  {:>3} {:>4} {:>4}  ← 比②多一次移动",
                 Trace::ctor, Trace::copy, Trace::move);

    Trace::ctor = Trace::copy = Trace::move = 0;
    {
        Holder h{Trace{5}};              // 临时直接构造进参数 → 再移动进成员
    }
    std::println("⑤ sink 参数 Holder(Trace)     {:>3} {:>4} {:>4}  ← 移动进成员，无法省略",
                 Trace::ctor, Trace::copy, Trace::move);

    // 结论要点
    std::println("\n规则：");
    std::println("  · 按值返回是默认答案 —— 编译器会消除拷贝，不要画蛇添足");
    std::println("  · 永远 return local; 而不是 return std::move(local);");
    std::println("  · 接收大对象：sink 用值参+move；只读用 const&；可变就地改用 &");
    return 0;
}
