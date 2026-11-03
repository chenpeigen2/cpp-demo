// 41 - std::array / std::deque / std::list / std::forward_list
#include <print>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <numeric>

int main() {
    // std::array：定长数组，栈上，零开销封装原始数组（知道自己的 size）
    std::array<int, 5> a{1, 2, 3, 4, 5};
    std::println("array size={} sum={} back={}", a.size(), std::accumulate(a.begin(), a.end(), 0), a.back());
    auto [x, y] = std::array{10, 20};        // 结构化绑定也适用

    // std::deque：双端队列，两端 O(1) 插删，也支持随机访问
    std::deque<int> d{2, 3};
    d.push_front(1); d.push_back(4);         // 两端都能进
    d.pop_back();
    std::println("deque: {} d[1]={}", d, d[1]);

    // std::list：双向链表，任意位置 O(1) 插删（若有迭代器），无随机访问
    std::list<int> l{1, 2, 3};
    auto it = std::next(l.begin());          // 指向 2
    l.insert(it, 99);                        // O(1) 插入
    l.remove(3);                             // 按值删除
    std::println("list: {}", l);

    // list 特有算法：成员版 sort（因为 std::sort 需要随机访问）
    std::list<int> ls{5, 2, 8, 1};
    ls.sort();
    ls.unique();                             // 去相邻重复
    std::println("sorted list: {}", ls);

    // std::forward_list：单向链表，更省内存，只有前向遍历
    std::forward_list<int> fl{1, 2, 3};
    fl.push_front(0);
    std::print("forward_list: ");
    for (int v : fl) std::print("{} ", v);
    std::println("");
    std::println("x={} y={}", x, y);
    return 0;
}
