// 40 - std::vector：动态数组，最常用的容器
#include <print>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v{3, 1, 4};
    v.push_back(1);                          // 尾部追加（均摊 O(1)）
    v.emplace_back(5);                       // 原地构造，少一次拷贝
    std::println("size={} capacity={} front={} back={}",
                 v.size(), v.capacity(), v.front(), v.back());

    // capacity 是已分配空间，size 是实际元素数；reserve 预分配避免反复扩容
    std::vector<int> r;
    r.reserve(100);
    std::println("reserve 后 capacity={}", r.capacity());

    // 随机访问 O(1)；越界检查用 at（抛异常）而非 []
    std::println("v[2]={} v.at(2)={}", v[2], v.at(2));

    // 插入/删除：中间位置 O(n)
    v.insert(v.begin() + 1, 99);             // 在下标1处插 99
    v.erase(v.begin());                      // 删除下标0
    std::println("insert/erase 后: {}", v);

    // C++23：append_range 批量追加
    v.append_range(std::vector{7, 8});
    std::println("append_range: {}", v);

    // 遍历三种写法
    for (int x : v) std::print("{} ", x);    // 范围 for
    for (std::size_t i = 0; i < v.size(); i++) std::print("|{}", v[i]);
    std::println("");

    // 二维 vector
    std::vector<std::vector<int>> grid(2, std::vector<int>(3, 0));
    grid[1][2] = 9;
    std::println("grid 2x3, grid[1][2]={}", grid[1][2]);

    // shrink_to_fit 释放多余容量；clear 清空（capacity 不变）
    v.shrink_to_fit();
    std::println("shrink 后 capacity={}", v.capacity());
    return 0;
}
