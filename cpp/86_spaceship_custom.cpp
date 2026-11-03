// 86 - 进阶：operator<=> 定制 —— 从 default 到 partial_ordering
#include <print>
#include <compare>
#include <cmath>
#include <string>
#include <array>
#include <algorithm>

// ---- 1) default：成员逐个按声明序比较 ----
struct Version {
    int major, minor, patch;
    auto operator<=>(const Version &) const = default;   // 一次生成全部 6 个运算符
};
static_assert(Version{1, 2, 3} < Version{1, 3, 0});
static_assert(Version{2, 0, 0} == Version{2, 0, 0});

// ---- 2) 自定义返回 strong_ordering：先比 A，A 相等再比 B ----
struct Employee {
    int level;
    std::string name;
    constexpr auto operator<=>(const Employee &o) const {
        if (auto c = level <=> o.level; c != 0) return c;  // 职级优先
        return name <=> o.name;                            // 再按名字
    }
    bool operator==(const Employee &o) const = default;    // == 与 <=> 解耦，可单独定
};
static_assert(Employee{5, "alice"} < Employee{5, "bob"});
static_assert(Employee{5, "bob"} < Employee{6, "aardvark"});

// ---- 3) weak_ordering：等价但不同（大小写不敏感）----
struct CaseInsensitive {
    char c;
    static constexpr char lower(char ch) {               // constexpr 版 tolower（仅 ASCII）
        return (ch >= 'A' && ch <= 'Z') ? char(ch - 'A' + 'a') : ch;
    }
    constexpr std::weak_ordering operator<=>(const CaseInsensitive &o) const {
        char a = lower(c), b = lower(o.c);
        return a < b ? std::weak_ordering::less
             : a > b ? std::weak_ordering::greater
                     : std::weak_ordering::equivalent;
    }
    constexpr bool operator==(const CaseInsensitive &o) const {
        return (*this <=> o) == std::weak_ordering::equivalent;
    }
};
static_assert(CaseInsensitive{'A'} == CaseInsensitive{'a'});   // 等价
static_assert(!(CaseInsensitive{'A'} == CaseInsensitive{'b'}));

// ---- 4) partial_ordering：NaN 使比较"无序"（浮点的本质）----
struct Temp {
    double celsius;
    std::partial_ordering operator<=>(const Temp &o) const { return celsius <=> o.celsius; }
    bool operator==(const Temp &o) const = default;
};

int main() {
    std::println("Version 字典序: {} {} {}",
                 Version{1, 9, 9} < Version{2, 0, 0},
                 Version{1, 2, 9} < Version{1, 3, 0},
                 Version{1, 2, 3} <=> Version{1, 2, 3} == std::strong_ordering::equal);

    std::println("Employee: (5,alice)<(5,bob)={}  (5,bob)<(6,a)={}",
                 Employee{5, "alice"} < Employee{5, "bob"},
                 Employee{5, "bob"} < Employee{6, "aardvark"});

    // partial_ordering 的 unordered：与 NaN 任何比较都不成立
    Temp hot{36.6}, nan{NAN};
    std::println("36.6 < NaN = {}（无序！不是 false 那么简单的布尔）",
                 (hot < nan) ? "true" : "false");
    std::println("NaN <=> NaN == unordered: {}",
                 (nan <=> nan) == std::partial_ordering::unordered);

    // 三类 ordering 的强度阶梯（strong ⊃ weak ⊃ partial）
    std::println("strong_ordering: 可代一切（-1/0/1 三态，且相等可替换）");
    std::println("weak_ordering:   等价≠相同（如大小写、忽略字段）");
    std::println("partial_ordering: 允许'不可比'（只有浮点会出现）");

    // 排序算法直接受益：一个 <=> 喂饱 sort/max/min/set 全家
    std::array<Version, 3> vs{Version{1, 10, 0}, Version{1, 2, 0}, Version{0, 9, 9}};
    std::ranges::sort(vs);
    std::println("排序后: {}.{}.{} < {}.{}.{} < {}.{}.{}",
                 vs[0].major, vs[0].minor, vs[0].patch,
                 vs[1].major, vs[1].minor, vs[1].patch,
                 vs[2].major, vs[2].minor, vs[2].patch);
    std::println("要点：写 <=> 不写 6 个运算符；返回类型声明你的'相等强度'；== 可独立覆写");
    return 0;
}
