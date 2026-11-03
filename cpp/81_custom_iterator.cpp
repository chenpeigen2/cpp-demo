// 81 - 进阶：手写 STL 迭代器 —— 让自定义容器接入 ranges
#include <print>
#include <array>
#include <ranges>
#include <algorithm>
#include <numeric>

// 环形缓冲区：固定容量，覆盖最旧数据
template <typename T, std::size_t N>
class Ring {
    std::array<T, N> buf_{};
    std::size_t head_ = 0;          // 最旧元素位置
    std::size_t size_ = 0;          // 当前元素数
public:
    void push(T v) {
        buf_[(head_ + size_) % N] = v;
        if (size_ == N) head_ = (head_ + 1) % N;   // 满了覆盖最旧
        else size_++;
    }
    std::size_t size() const { return size_; }

    // ---- 迭代器：满足 forward_iterator 概念（这是接入 ranges 的门票）----
    class Iterator {
        const Ring *ring_ = nullptr;
        std::size_t idx_ = 0;                       // 相对 head 的第几个元素
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;

        Iterator() = default;                       // weakly_incrementable 要求默认构造
        Iterator(const Ring *r, std::size_t i) : ring_(r), idx_(i) {}

        const T &operator*() const { return ring_->buf_[(ring_->head_ + idx_) % N]; }
        Iterator &operator++() { idx_++; return *this; }
        Iterator operator++(int) { auto t = *this; ++(*this); return t; }
        bool operator==(const Iterator &) const = default;
    };

    Iterator begin() const { return {this, 0}; }
    Iterator end() const { return {this, size_}; }
};

// 编译期验证：概念检查就是"迭代器面试"
static_assert(std::forward_iterator<Ring<int, 4>::Iterator>);

int main() {
    Ring<int, 5> r;
    for (int i = 1; i <= 7; i++) r.push(i);        // 1..7，前两个被覆盖
    std::println("环形缓冲(容量5) 内容: {}", r);    // 3 4 5 6 7

    // 因为满足了概念，标准库算法/视图全部免费可用
    std::println("ranges::for_each 平方: ");
    std::ranges::for_each(r, [](int v) { std::print("{} ", v * v); });
    std::println("");

    auto evens = r | std::views::filter([](int v) { return v % 2 == 0; });
    std::println("过滤偶数: {}", evens);

    std::println("sum={}", std::accumulate(r.begin(), r.end(), 0));
    std::println("count>4={}", std::ranges::count_if(r, [](int v) { return v > 4; }));

    // 反向遍历用 views::reverse 不行（forward 迭代器没有 bidirectional），
    // 概念系统在编译期精确告诉你缺什么 —— 这就是"按能力付费"
    std::println("概念检查: forward={} bidirectional={}",
                 std::forward_iterator<Ring<int, 5>::Iterator>,
                 std::bidirectional_iterator<Ring<int, 5>::Iterator>);

    // 迭代器失效纪律：push 覆盖时 head 前移，旧迭代器仍按"相对第 i 个"取值 → 语义稳定
    r.push(99);
    std::println("push(99) 后: {}（覆盖了最旧的 3）", r);
    return 0;
}
