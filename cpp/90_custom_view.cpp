// 90 - 进阶：手写 ranges 视图 —— 让自己的变换接入 | 管道
#include <print>
#include <ranges>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// ---- 目标：every_other 视图，支持 v | every_other | take(3) 管道 ----

// 1) 视图本体：包一个底层 range，隔一个取一个
template <std::ranges::view R>
class EveryOther : public std::ranges::view_interface<EveryOther<R>> {
    R base_;                       // view_interface 免费送 empty()/front()/operator bool()
public:
    EveryOther() requires std::default_initializable<R> = default;
    constexpr explicit EveryOther(R base) : base_(std::move(base)) {}

    // 迭代器：持有当前位置和终点，++ 时跳一步
    struct Iterator {
        std::ranges::iterator_t<R> cur_, end_;
        using value_type = std::ranges::range_value_t<R>;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;

        decltype(auto) operator*() const { return *cur_; }
        Iterator &operator++() {
            ++cur_;
            if (cur_ != end_) ++cur_;          // 隔一个
            return *this;
        }
        Iterator operator++(int) { auto t = *this; ++(*this); return t; }
        bool operator==(const Iterator &) const = default;   // C++20 无需写 !=
    };

    Iterator begin() const { return {std::ranges::begin(base_), std::ranges::end(base_)}; }
    Iterator end() const { return {std::ranges::end(base_), std::ranges::end(base_)}; }
};

// 2) 管道闭包：让 v | every_other 成立（C++20 pipe-support 的标准写法）
struct EveryOtherFn {
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R &&r) const {
        return EveryOther<std::views::all_t<R>>(std::forward<R>(r));  // all_t 统一 左值/右值
    }
};
template <std::ranges::viewable_range R>
constexpr auto operator|(R &&r, EveryOtherFn) {
    return EveryOtherFn{}(std::forward<R>(r));
}
inline constexpr EveryOtherFn every_other{};

// 编译期验证：它是真 view（可组合、惰性、O(1) 拷贝）
static_assert(std::ranges::view<EveryOther<std::ranges::ref_view<std::vector<int>>>>);

int main() {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0);              // 0..9

    // 基本使用
    std::println("原序列:      {}", v);
    std::println("every_other: {}", v | every_other);

    // 惰性组合：不产生任何中间容器
    std::println("管道组合:    {}", v | every_other | std::views::take(2));
    std::println("再过滤:      {}", v | every_other
                                       | std::views::filter([](int x) { return x % 3 == 0; }));
    std::println("物化:        {}", v | every_other | std::ranges::to<std::vector>());

    // 视图是"描述"不是"数据"：底层变了，视图跟着变
    auto view = v | every_other;
    v[0] = 100;
    std::println("改底层数据后视图首元素 = {}（视图只是个镜头）", *view.begin());

    // 与 ranges 算法互动（概念检查自动通过）
    auto n = std::ranges::count_if(view, [](int x) { return x > 4; });
    std::println("ranges::count_if(>4) = {}", n);

    // 这 70 行就是 std::views 里每个视图的共同骨架：
    //   1. 包 R base_   2. 迭代器定义 ++/*   3. 闭包 + operator|   4. view_interface 送接口
    std::println("要点：视图=惰性+可组合+零中间分配；把算法变成'声明式数据流'");
    return 0;
}
