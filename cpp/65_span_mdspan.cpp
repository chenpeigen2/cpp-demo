// 65 - <span> <mdspan>：非拥有视图（C++20/C++23）
#include <print>
#include <span>
#include <mdspan>
#include <vector>
#include <array>
#include <numeric>

// span 参数：接受 vector/array/原始数组/指针+长度，零拷贝
static int sum(std::span<const int> data) {         // const span = 只读视图
    int total = 0;
    for (int v : data) total += v;
    return total;
}

static void print_span(std::span<int> sp) {
    for (int v : sp) std::print("{} ", v);
    std::println("");
}

int main() {
    std::vector<int> v(6);
    std::iota(v.begin(), v.end(), 1);               // 1 2 3 4 5 6

    // span 从各种来源构造
    std::span<int> sp{v};                           // 从 vector
    std::array<int, 3> arr{7, 8, 9};
    std::span<int> spa{arr};                        // 从 std::array
    int raw[] = {10, 20};
    std::span<int> spr{raw};                        // 从原始数组（自动带长度）

    std::println("from vector: sum={}", sum(sp));
    std::println("from array: sum={}", sum(spa));
    std::println("from raw: sum={}", sum(spr));

    // 子视图：不拷贝，还是同一块内存
    print_span(sp.first(2));
    print_span(sp.last(2));
    print_span(sp.subspan(1, 3));

    // 修改通过视图直达底层
    sp[0] = 100;
    std::println("改 sp[0] 后 v[0]={}", v[0]);

    // 元信息
    std::println("size={} size_bytes={} empty={}", sp.size(), sp.size_bytes(), sp.empty());
    std::println("front={} back={}", sp.front(), sp.back());

    // 静态长度 span：编译期知道大小
    std::span<int, 6> fixed{v};
    std::println("fixed span 长度={}", fixed.size());

    // 只读字节视图：检查对象表示
    auto bytes = std::as_bytes(sp);
    std::println("字节视图大小={}", bytes.size());

    // ---- mdspan (C++23)：多维视图，底层仍是一维内存 ----
    std::mdspan m{v.data(), std::extents{2, 3}};    // 2行3列看待6个元素
    m[1, 2] = 99;                                   // C++23 多维下标运算符
    std::println("mdspan m[1,2]={}（即 v[5]）", m[1, 2]);
    std::println("v[5]={}", v[5]);
    std::println("extents: {} x {}", m.extent(0), m.extent(1));
    return 0;
}
