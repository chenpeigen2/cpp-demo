// 87 - 进阶：flat_map vs map 实测 —— 两种内存布局的胜负手
#include <print>
#include <flat_map>
#include <map>
#include <unordered_map>
#include <string>
#include <chrono>
#include <random>
#include <numeric>
#include <algorithm>

template <typename MapT>
static void fill(MapT &m, const std::vector<int> &keys) {
    for (int i = 0; i < (int)keys.size(); i++) m[keys[i]] = i * 2;
}

template <typename MapT>
static double bench_lookup(const MapT &m, const std::vector<int> &probe_keys, int iters) {
    volatile long long sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int it = 0; it < iters; it++)
        for (int k : probe_keys) {
            auto it2 = m.find(k);
            if (it2 != m.end()) sink += it2->second;
        }
    auto t1 = std::chrono::steady_clock::now();
    (void)sink;
    double ops = (double)iters * probe_keys.size();
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
}

int main() {
    constexpr int N = 100'000;
    std::mt19937 rng(42);
    std::vector<int> keys(N);
    std::iota(keys.begin(), keys.end(), 0);
    std::shuffle(keys.begin(), keys.end(), rng);       // 乱序插入

    std::flat_map<int, int> flat;
    std::map<int, int> tree;
    std::unordered_map<int, int> hash;
    fill(flat, keys);
    fill(tree, keys);
    fill(hash, keys);

    std::println("── 插入 {} 个乱序键 ──", N);
    auto t0 = std::chrono::steady_clock::now();
    std::flat_map<int, int> f2;
    for (int k : keys) f2[k] = 1;
    auto t1 = std::chrono::steady_clock::now();
    std::map<int, int> m2;
    for (int k : keys) m2[k] = 1;
    auto t2 = std::chrono::steady_clock::now();
    std::unordered_map<int, int> h2;
    h2.reserve(N);
    for (int k : keys) h2[k] = 1;
    auto t3 = std::chrono::steady_clock::now();
    auto ms = [](auto d) { return std::chrono::duration<double, std::milli>(d).count(); };
    std::println("flat_map  {:>7.1f} ms（每次插入都是有序 vector → O(n) 搬移）", ms(t1 - t0));
    std::println("map       {:>7.1f} ms（红黑树 O(log n)，节点堆分配）", ms(t2 - t1));
    std::println("hash_map  {:>7.1f} ms（均摊 O(1)）", ms(t3 - t2));

    // 查询基准：命中一半、未命中一半
    std::vector<int> probes;
    for (int i = 0; i < N; i += 2) probes.push_back(i);            // 存在
    for (int i = 1; i < N; i += 2) probes.push_back(i + N);        // 不存在
    std::shuffle(probes.begin(), probes.end(), rng);
    constexpr int ITERS = 3;

    std::println("\n── 查找（{} 次查询均摊 ns/次）──", ITERS * probes.size());
    std::println("flat_map  {:>7.1f}（连续内存：二分 + 缓存/预取全开）",
                 bench_lookup(flat, probes, ITERS));
    std::println("map       {:>7.1f}（每层一次指针跳转 = 一次缓存未命中）",
                 bench_lookup(tree, probes, ITERS));
    std::println("hash_map  {:>7.1f}（哈希 + 桶跳转）", bench_lookup(hash, probes, ITERS));

    std::println("\n内存占用估算（键+值各 4B）：");
    std::println("  flat_map ≈ {} KB（就两个 vector）", (N * 8 + 1023) / 1024);
    std::println("  map      ≈ {} KB（每节点 3 指针 + 颜色 + 对齐）", N * 40 / 1024);
    std::println("\n选型：只读/批量构建 → flat_map 常胜；频繁随机插入删除 → map；纯点查 → hash_map");
    return 0;
}
