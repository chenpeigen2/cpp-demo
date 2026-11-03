// 74 - 进阶：CRTP 静态多态 —— 零虚函数开销的"继承复用"
#include <print>
#include <chrono>
#include <vector>

// CRTP：基类以派生类为模板参数，编译期就知道"自己是谁"
template <typename Derived>
class Shape {
public:
    double area() const { return self().area_impl(); }        // 编译期分派，无虚表
    double perimeter() const { return self().perimeter_impl(); }

    // 模板方法模式：基类定义算法骨架，细节由派生类提供
    void describe() const {
        std::println("  面积={:.2f} 周长={:.2f}（编译期绑定）", area(), perimeter());
    }
private:
    const Derived &self() const { return static_cast<const Derived &>(*this); }
};

class Circle : public Shape<Circle> {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area_impl() const { return 3.14159265358979 * r_ * r_; }
    double perimeter_impl() const { return 2 * 3.14159265358979 * r_; }
};

class Rect : public Shape<Rect> {
    double w_, h_;
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area_impl() const { return w_ * h_; }
    double perimeter_impl() const { return 2 * (w_ + h_); }
};

// ---- 性能对比：虚函数 vs CRTP，N 次调用总耗时 ----
struct VBase { virtual double area() const = 0; virtual ~VBase() = default; };
struct VSquare : VBase {
    double s;
    explicit VSquare(double s) : s(s) {}
    double area() const override { return s * s; }
};

template <typename T>
double crtp_total(const T &s, long n) {
    double t = 0;
    for (long i = 0; i < n; i++) t += s.area();
    return t;
}

double virtual_total(const VBase &s, long n) {
    double t = 0;
    for (long i = 0; i < n; i++) t += s.area();
    return t;
}

int main() {
    Circle c{2.0};
    Rect r{3.0, 4.0};
    std::println("圆:"); c.describe();
    std::println("矩形:"); r.describe();

    // 性能对比数据
    constexpr long N = 50'000'000;
    auto t0 = std::chrono::steady_clock::now();
    double a = crtp_total(Rect{1.5, 2.5}, N);
    auto t1 = std::chrono::steady_clock::now();
    VSquare vs{1.5};
    double b = virtual_total(vs, N);
    auto t2 = std::chrono::steady_clock::now();
    auto us = [](auto d) { return std::chrono::duration<double, std::micro>(d).count(); };
    std::println("\n调用 {} 次 area()：", N);
    std::println("  CRTP（编译期绑定）: {:>8.0f} µs  结果={}", us(t1 - t0), a);
    std::println("  virtual（虚表分派）: {:>8.0f} µs  结果={}", us(t2 - t1), b);
    std::println("（差值≈每次调用几 ns 的间接跳转 + 优化器无法内联的代价；\n"
                 "  注意两者都逃不过函数调用本身，瓶颈场景要测自己的代码）");

    std::println("CRTP 适用：小而热的多态调用、需要内联的算法骨架；虚函数适合真正的运行期多态集合");
    return 0;
}
