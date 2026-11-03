// 75 - 进阶：类型擦除三种写法 —— function/模板/手写 vtable（std::any 的原理）
#include <print>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

struct Duck   { void quack() const { std::println("  嘎嘎"); } };
struct Robot  { void quack() const { std::println("  哔——"); } };
struct Person { void quack() const { std::println("  模仿鸭子叫"); } };

// ---- 写法一：手写类型擦除（外部 vtable，值语义，std::function 的核心原理）----
class Quacker {
    struct Concept {                                 // 内部接口（非虚公开）
        virtual ~Concept() = default;
        virtual void do_quack() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };
    template <typename T>
    struct Model final : Concept {                   // 包一层把 T 适配到接口
        T obj_;
        explicit Model(T o) : obj_(std::move(o)) {}
        void do_quack() const override { obj_.quack(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(obj_);
        }
    };
    std::unique_ptr<Concept> self_;
public:
    template <typename T>                            // 任何有 quack() 的类型都能装
    Quacker(T t) : self_(std::make_unique<Model<T>>(std::move(t))) {}
    Quacker(const Quacker &o) : self_(o.self_->clone()) {}   // 深拷贝 → 值语义！
    void quack() const { self_->do_quack(); }
};

// ---- 写法二：std::function 擦除"调用签名" ----
// ---- 写法三：模板参数（编译期擦除，最快但每类型实例化一份代码）----
template <typename T>
static void quack_n(const T &q, int n) { for (int i = 0; i < n; i++) q.quack(); }

int main() {
    // 手写版：异构容器 + 值语义拷贝
    std::vector<Quacker> zoo;
    zoo.emplace_back(Duck{});
    zoo.emplace_back(Robot{});
    zoo.emplace_back(Person{});
    std::println("异构集合（每个元素类型不同，容器却 homogeneous）：");
    for (const auto &q : zoo) q.quack();

    Quacker copy = zoo[0];               // 拷贝的是 Duck，不是 Quacker 壳
    std::println("拷贝后调用:"); copy.quack();

    // std::function：擦除成"无参可调用"
    std::vector<std::function<void()>> tasks;
    tasks.push_back([] { std::println("  lambda 任务"); });
    int data = 42;
    tasks.push_back([data] { std::println("  捕获任务 {}", data); });
    for (auto &t : tasks) t();

    // 三种方式的成本数据
    constexpr long N = 10'000'000;
    Duck d{};
    auto t0 = std::chrono::steady_clock::now();
    quack_n(d, N);                                        // 模板：编译期绑定，可内联
    auto t1 = std::chrono::steady_clock::now();
    for (long i = 0; i < N; i++) d.quack();               // 直接调用（对照）
    auto t2 = std::chrono::steady_clock::now();
    auto us = [](auto x) { return std::chrono::duration<double, std::micro>(x).count(); };
    std::println("调用 {} 次（空操作被内联后近似直调）：模板 {:>6.0f}µs / 直调 {:>6.0f}µs",
                 N, us(t1 - t0), us(t2 - t1));
    std::println("成本排序：模板 < 手写擦除(一次虚调用+堆) < std::function(同+更大开销) < std::any(再加类型查询)");
    std::println("选择：运行期异构集合→手写/function；性能热路径→模板；只存任意值→any/variant");
    return 0;
}
