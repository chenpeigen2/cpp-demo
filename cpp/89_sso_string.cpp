// 89 - 进阶：std::string 的 SSO（短字符串优化）—— 栈上的字符串
#include <print>
#include <string>
#include <chrono>
#include <cstring>

// 判断指针在栈附近还是堆上：与一个栈变量比距离
static const char *where(const void *p, const void *stack_ref) {
    auto d = std::abs(reinterpret_cast<std::intptr_t>(p) - reinterpret_cast<std::intptr_t>(stack_ref));
    return d < (1 << 20) ? "栈(SSO)" : "堆";
}

int main() {
    int stack_ref = 0;                            // 栈参考点

    std::println("── SSO 边界探测（libc++：短字符串直接存在对象内部）──");
    for (int n : {5, 15, 22, 23, 50}) {
        std::string s(n, 'x');
        std::println("长度 {:>2}: capacity={:>3} data={}（{}）",
                     n, s.capacity(), (void *)s.data(), where(s.data(), &stack_ref));
    }
    // sizeof(std::string) = 24：SSO 期间这 24 字节既当栈缓冲(22B+1)又当 {指针,大小,容量}
    std::println("sizeof(std::string) = {} —— 前 22 字符免费住栈里，无堆分配", sizeof(std::string));

    // reserve 超过 SSO 上限 → 立刻上堆；shrink_to_fit 可能搬回栈？libc++ 不会缩回 SSO
    std::string s = "short";
    std::println("\n\"short\": data={}（{}）", (void *)s.data(), where(s.data(), &stack_ref));
    s.reserve(1000);
    std::println("reserve(1000) 后: data={}（{}）", (void *)s.data(), where(s.data(), &stack_ref));
    s = "tiny";
    std::println("重新赋短值后: data={}（{}，堆缓冲不会自动还回去）", (void *)s.data(), where(s.data(), &stack_ref));

    // ---- 性能差异实测：SSO 字符串 vs 堆字符串 ----
    constexpr int N = 1'000'000;
    auto bench = [&](int len) {
        auto t0 = std::chrono::steady_clock::now();
        volatile std::size_t sink = 0;
        for (int i = 0; i < N; i++) {
            std::string v(len, 'x');
            sink += v.size();
        }                                          // 构造+析构各一次
        auto t1 = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::nano>(t1 - t0).count() / N;
    };
    std::println("\n构造+析构 {} 次：", N);
    std::println("  长度 10（SSO，零分配）: {:>5.1f} ns/次", bench(10));
    std::println("  长度 25（堆分配回收）: {:>5.1f} ns/次", bench(25));
    std::println("  长度 100（堆分配回收）: {:>5.1f} ns/次", bench(100));

    // 工程含义
    std::println("\n工程含义：");
    std::println("  · 短键（ID/枚举名/hash）用 std::string 几乎免费，不必怕");
    std::println("  · 大文本传参用 string_view / const&，避免复制 24 字节+堆数据");
    std::println("  · string 拷贝 = 深拷贝（堆数据），移动 = 只偷 24 字节（见 85 号实测）");
    std::println("  · SSO 阈值是实现细节（libc++ 22 字节，libstdc++ 15 字节），别依赖具体值");
    return 0;
}
