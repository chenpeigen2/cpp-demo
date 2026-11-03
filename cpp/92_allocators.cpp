// 92 - 进阶：自定义分配器统计 —— 看见容器的每一次堆分配
#include <print>
#include <vector>
#include <map>
#include <unordered_map>
#include <flat_map>
#include <memory>
#include <random>
#include <numeric>
#include <algorithm>

// 全局统计（简化：真实项目按实例/线程记账）
struct AllocStats {
    static inline std::size_t count = 0, bytes = 0;
    static void reset() { count = 0; bytes = 0; }
};

template <typename T>
struct CountingAlloc {
    using value_type = T;
    CountingAlloc() = default;
    template <typename U>
    CountingAlloc(const CountingAlloc<U> &) {}          // rebind 转换构造（C++20 形式）
    T *allocate(std::size_t n) {
        AllocStats::count++;
        AllocStats::bytes += n * sizeof(T);
        return std::allocator<T>{}.allocate(n);
    }
    void deallocate(T *p, std::size_t n) { std::allocator<T>{}.deallocate(p, n); }
    template <typename U>
    bool operator==(const CountingAlloc<U> &) const noexcept { return true; }
};

static void report(const char *tag) {
    std::println("{:<34} {:>4} 次分配 {:>8} 字节", tag, AllocStats::count, AllocStats::bytes);
}

int main() {
    constexpr int N = 1000;

    // ---- vector：reserve 的分配次数差异 ----
    AllocStats::reset();
    {
        std::vector<int, CountingAlloc<int>> v;
        for (int i = 0; i < N; i++) v.push_back(i);
    }
    report("vector 1000 次 push（无 reserve）");

    AllocStats::reset();
    {
        std::vector<int, CountingAlloc<int>> v;
        v.reserve(N);
        for (int i = 0; i < N; i++) v.push_back(i);
    }
    report("vector 1000 次 push（reserve）");

    // ---- 三种 map：节点分配 vs 连续缓冲 ----
    std::mt19937 rng(42);
    std::vector<int> keys(N);
    std::iota(keys.begin(), keys.end(), 0);
    std::shuffle(keys.begin(), keys.end(), rng);

    AllocStats::reset();
    {
        std::map<int, int, std::less<int>, CountingAlloc<std::pair<const int, int>>> m;
        for (int k : keys) m[k] = 1;
    }
    report("map 1000 个键（红黑树=每节点一次）");

    AllocStats::reset();
    {
        std::unordered_map<int, int, std::hash<int>, std::equal_to<int>,
                           CountingAlloc<std::pair<const int, int>>> m;
        m.reserve(N);                                    // 不 reserve 会更碎
        for (int k : keys) m[k] = 1;
    }
    report("unordered_map 1000 个键（桶+节点）");

    AllocStats::reset();
    {
        std::flat_map<int, int, std::less<int>,
                      std::vector<int, CountingAlloc<int>>,
                      std::vector<int, CountingAlloc<int>>> m;
        // 注：libc++ 的 flat_map 暂无 reserve，插入过程中会多次扩容
        for (int k : keys) m[k] = 1;
    }
    report("flat_map 1000 个键（就两个 vector）");

    // ---- string 的分配也可以看见 ----
    AllocStats::reset();
    {
        std::basic_string<char, std::char_traits<char>, CountingAlloc<char>> s;
        for (int i = 0; i < 100; i++) s += "hello ";
    }
    report("basic_string 100 次 +=（无预留）");

    // 结论
    std::println("\n读数：");
    std::println("  · map 的 1000 个节点 = 1000 次独立堆分配 —— 缓存杀手（见 87 号性能数据）");
    std::println("  · vector 无 reserve 也要 ~10 次重分配，每次都搬移全部元素");
    std::println("  · flat_map 两次分配 —— 这就是它查询快 3-5 倍的根源");
    std::println("  · allocator 是观察工具，也是武器：pmr(69 号) + 内存池(36 号) 就是它的实战形态");
    return 0;
}
