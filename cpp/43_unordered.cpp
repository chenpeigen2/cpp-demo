// 43 - 无序容器：unordered_map / unordered_set（哈希表）
#include <print>
#include <unordered_map>
#include <unordered_set>
#include <string>

// 自定义类型的哈希：要同时提供 operator== 和哈希函数
struct Point {
    int x, y;
    bool operator==(const Point &) const = default;
};
struct PointHash {
    std::size_t operator()(const Point &p) const {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

int main() {
    // unordered_map：哈希表，平均 O(1) 查找，无序遍历
    std::unordered_map<std::string, int> stock{{"apple", 3}, {"pear", 5}};
    stock["banana"] = 2;
    stock.insert_or_assign("apple", 4);

    std::print("遍历(顺序不定): ");
    for (const auto &[k, v] : stock) std::print("{}:{} ", k, v);
    std::println("");

    std::println("bucket_count={} load_factor={:.2f}",
                 stock.bucket_count(), stock.load_factor());  // 哈希表内部状态

    // C++23：insert_range 批量插入；contains 同样可用
    stock.insert_range(std::unordered_map<std::string, int>{{"fig", 1}});
    std::println("contains fig={} erase apple={}",
                 stock.contains("fig"), stock.erase("apple"));

    // try_emplace：键已存在时不构造值（比 operator[] 高效且不覆盖）
    auto [it, inserted] = stock.try_emplace("pear", 99);
    std::println("try_emplace pear: inserted={} value={}", inserted, it->second);

    // unordered_set：哈希集合，O(1) 的 contains
    std::unordered_set<int> us{1, 2, 3, 2, 1};
    std::println("set size={} contains 3={}", us.size(), us.contains(3));

    // 自定义类型做键
    std::unordered_map<Point, std::string, PointHash> pmap{{{1, 2}, "left-up"}};
    std::println("point(1,2)={}", pmap[Point{1, 2}]);

    // unordered_multiset / unordered_multimap：允许重复键的哈希版本
    std::unordered_multiset<int> ums{1, 1, 2};
    std::println("ums count(1)={}", ums.count(1));
    return 0;
}
