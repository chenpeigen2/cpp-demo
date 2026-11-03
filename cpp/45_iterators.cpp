// 45 - 迭代器：五种类别与迭代器工具
#include <print>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // begin/end 迭代器像"泛化指针"：解引用、前进、比较
    for (auto it = v.begin(); it != v.end(); ++it)
        std::print("{} ", *it);
    std::println("");

    // const_iterator / cbegin：只读遍历
    auto cit = v.cbegin();
    std::println("cbegin={}", *cit);

    // 反向迭代器
    std::print("reverse: ");
    for (auto it = v.rbegin(); it != v.rend(); ++it) std::print("{} ", *it);
    std::println("");

    // 迭代器算术：随机访问迭代器支持 it+n / it1-it2
    auto it = v.begin();
    std::println("v[3]={} 距离={}", it[3], v.end() - v.begin());
    std::advance(it, 2);                     // 前进 2 步
    std::println("advance(2) 后={}", *it);
    std::println("next(it)={} prev(it)={}", *std::next(it), *std::prev(it));

    // distance：两个迭代器之间的距离（list 只有双向，不能 it1-it2）
    std::list<int> l{1, 2, 3};
    std::println("list distance={}", std::distance(l.begin(), l.end()));

    // 插入迭代器：把"赋值"变成"插入"
    std::vector<int> dst;
    std::copy(v.begin(), v.end(), std::back_inserter(dst));   // 尾插
    std::println("back_inserter 拷贝: {}", dst);

    // 流迭代器：直接迭代标准输入输出
    std::print("ostream_iterator: ");
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ","));
    std::println("");

    // 迭代器失效提醒：vector 扩容会使全部迭代器失效
    // （示例见 40_vector.cpp 的 reserve 用法）
    return 0;
}
