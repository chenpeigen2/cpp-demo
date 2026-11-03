// 34 - 运算符重载：成员 vs 非成员
#include <print>

class Vec2 {
public:
    double x{}, y{};
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2 operator+(const Vec2 &o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2 &o) const { return {x - o.x, y - o.y}; }
    Vec2 operator-() const { return {-x, -y}; }            // 一元负号
    Vec2 &operator+=(const Vec2 &o) { x += o.x; y += o.y; return *this; }
    double operator[](int i) const { return i == 0 ? x : y; }  // 下标
    explicit operator bool() const { return x != 0 || y != 0; } // 转布尔（explicit 防意外转换）

    // 比较运算符 C++20 起可以只写 ==
    bool operator==(const Vec2 &o) const = default;
};

// 非成员：左操作数不是 Vec2 时也能工作（double * Vec2）
Vec2 operator*(double k, const Vec2 &v) { return {k * v.x, k * v.y}; }

// 流输出必须是非成员（左操作数是 ostream）
#include <iostream>
std::ostream &operator<<(std::ostream &os, const Vec2 &v) {
    return os << "(" << v.x << "," << v.y << ")";
}

int main() {
    Vec2 a{1, 2}, b{3, 4};
    Vec2 c = a + b;
    std::println("c={}{}{}{}", c.x, ',', c.y, ' ');
    std::println("-c={} {}", (-c).x, (-c).y);
    a += b;
    std::println("a.x={} a[1]={}", a.x, a[1]);
    std::println("3*a=({},{}), bool={}", (3.0 * a).x, (3.0 * a).y, a ? "true" : "false");
    std::cout << "流输出: " << a << '\n';
    std::println("a==b? {}", a == b);
    return 0;
}
