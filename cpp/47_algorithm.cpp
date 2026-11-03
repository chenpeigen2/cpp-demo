// 47 - <algorithm>：常用算法全景
#include <print>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>

int main() {
    std::vector<int> v{5, 2, 8, 1, 9, 3};

    // 查找族
    auto it = std::find(v.begin(), v.end(), 8);
    std::println("find(8) 位置={}", it - v.begin());
    std::println("count(奇数)={}",
                 std::count_if(v.begin(), v.end(), [](int x) { return x % 2; }));
    auto [min, max] = std::minmax_element(v.begin(), v.end());
    std::println("min={} max={}", *min, *max);

    // 变换族
    std::vector<int> doubled;
    std::transform(v.begin(), v.end(), std::back_inserter(doubled),
                   [](int x) { return x * 2; });
    std::println("transform x2: {}", doubled);

    // 排序族
    std::sort(v.begin(), v.end(), std::greater<>{});         // 降序
    std::println("sort 降序: {}", v);
    std::partial_sort(v.begin(), v.begin() + 3, v.end());    // 前3个最小有序
    std::println("partial_sort 前3: {}", std::vector(v.begin(), v.begin() + 3));
    std::nth_element(v.begin(), v.begin() + 2, v.end());     // 第2位就位（快选）
    std::println("nth_element 后 v[2]={}", v[2]);

    // 数值族（<numeric>）
    std::println("accumulate={}", std::accumulate(v.begin(), v.end(), 0));
    std::println("inner_product={}",
                 std::inner_product(v.begin(), v.begin() + 2, v.begin() + 2, 0));
    std::vector<int> parts;
    std::exclusive_scan(v.begin(), v.begin() + 4, std::back_inserter(parts), 0);
    std::println("exclusive_scan(前缀和): {}", parts);
    std::println("gcd lcm: {} {}", std::gcd(12, 18), std::lcm(4, 6));

    // 分区族
    std::vector<int> w{1, 2, 3, 4, 5, 6};
    auto mid = std::partition(w.begin(), w.end(), [](int x) { return x % 2 == 0; });
    std::println("partition 偶数在前: {} (分界在第{}位)", w, mid - w.begin());
    std::println("is_sorted={}", std::is_sorted(w.begin(), mid));

    // 其他高频
    std::vector<int> a{1, 2, 3}, b{1, 2, 4};
    std::println("mismatch 首个差异在 b[2]={}", *std::mismatch(a.begin(), a.end(), b.begin()).second);
    std::string s = "Hello";
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    std::println("toupper: {}", s);
    std::println("all/any/none: {} {} {}",
                 std::all_of(v.begin(), v.end(), [](int x) { return x >= 0; }),
                 std::any_of(v.begin(), v.end(), [](int x) { return x > 100; }),
                 std::none_of(v.begin(), v.end(), [](int x) { return x < 0; }));
    return 0;
}
