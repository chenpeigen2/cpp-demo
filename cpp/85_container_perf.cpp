// 85 - 进阶：容器性能数据 —— reserve/拷贝/移动/遍历的实测差距
#include <print>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <numeric>
#include <cmath>

using Clock = std::chrono::steady_clock;
static double ns_per(Clock::time_point t0, Clock::time_point t1, double ops) {
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
}

struct Big {                       // 模拟大对象：拷贝贵
    std::string data;
    explicit Big(int n) : data(n, 'x') {}
    Big(const Big &) { std::this_thread::sleep_for(std::chrono::microseconds(50)); }
    Big(Big &&) = default;
};

int main() {
    constexpr int N = 100'000;

    // ---- 1) push_back：reserve 与否 ----
    auto t0 = Clock::now();
    std::vector<int> a;
    for (int i = 0; i < N; i++) a.push_back(i);
    auto t1 = Clock::now();
    std::vector<int> b;
    b.reserve(N);
    for (int i = 0; i < N; i++) b.push_back(i);
    auto t2 = Clock::now();
    std::println("push_back {} 个 int：", N);
    std::println("  无 reserve  {:>8.1f} ns/个（约 {} 次重分配搬移）",
                 ns_per(t0, t1, N), (int)std::log2((double)N) - 2);
    std::println("  有 reserve  {:>8.1f} ns/个", ns_per(t1, t2, N));

    // ---- 2) vector 拷贝 vs 移动 ----
    std::vector<std::string> src;
    for (int i = 0; i < 1000; i++) src.emplace_back(1000, 'x');
    t0 = Clock::now();
    auto copy1 = src;                                  // 深拷贝：1000 次字符串分配
    t1 = Clock::now();
    auto move1 = std::move(src);                       // O(1)：偷三个指针
    t2 = Clock::now();
    std::println("\nvector<string>(1000×1000B)：");
    std::println("  拷贝 {:>9.0f} ns   移动 {:>5.0f} ns（差 {} 倍）",
                 ns_per(t0, t1, 1), ns_per(t1, t2, 1),
                 (long long)(ns_per(t0, t1, 1) / ns_per(t1, t2, 1)));

    // ---- 3) 遍历：vector vs list（缓存局部性）----
    std::vector<int> vec(N);
    std::iota(vec.begin(), vec.end(), 0);
    std::list<int> lst(vec.begin(), vec.end());
    t0 = Clock::now();
    long long s1 = 0;
    for (int v : vec) s1 += v;
    t1 = Clock::now();
    long long s2 = 0;
    for (int v : lst) s2 += v;
    t2 = Clock::now();
    std::println("\n遍历求和 {} 个 int：", N);
    std::println("  vector {:>7.1f} ns/个（连续内存，预取器友好）", ns_per(t0, t1, N));
    std::println("  list   {:>7.1f} ns/个（每节点一次指针跳转+缓存未命中）", ns_per(t1, t2, N));
    std::println("  （校验 {} == {}）—— 除'req O(1) 中间插删'外，list 几乎总是输", s1, s2);

    // ---- 4) 字符串拼接策略 ----
    constexpr int PARTS = 2000;
    auto bench_concat = [&](auto strategy) -> double {
        auto s = Clock::now();
        std::string out;
        for (int i = 0; i < PARTS; i++) strategy(out, i);
        auto e = Clock::now();
        if (out.empty()) std::println("");
        return ns_per(s, e, PARTS);
    };
    std::println("\n拼接 {} 段字符串：", PARTS);
    std::println("  += 无预留   {:>7.0f} ns/段", bench_concat([](std::string &o, int i) {
                     o += std::to_string(i); }));
    std::println("  += 有预留   {:>7.0f} ns/段", bench_concat([](std::string &o, int i) {
                     if (o.capacity() < 8192) o.reserve(8192);
                     o += std::to_string(i); }));
    std::println("  format      {:>7.0f} ns/段", bench_concat([](std::string &o, int i) {
                     o += std::format("[{}]", i); }));

    // ---- 5) erase 惯用法对比 ----
    std::vector<int> with_holes(N);
    std::iota(with_holes.begin(), with_holes.end(), 0);
    t0 = Clock::now();
    std::erase_if(with_holes, [](int v) { return v % 2 == 0; });   // C++20 一步到位
    t1 = Clock::now();
    std::println("\nerase_if 删一半({}→{}): {:.0f} ns/元素", N, with_holes.size(), ns_per(t0, t1, N));
    std::println("结论：vector+算法 是默认答案；数据决定一切，怀疑就测");
    return 0;
}
