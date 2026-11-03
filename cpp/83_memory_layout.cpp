// 83 - 进阶：对象内存布局 —— 空类、EBO、对齐、padding 数据表
#include <print>
#include <cstddef>
#include <variant>
#include <cstdint>

struct Empty {};
struct WithFunc { void f() {} };                    // 成员函数不占对象空间（存在代码段）
struct WithVirt { virtual void f() {} };
struct WithVirtDtor { virtual ~WithVirtDtor() {} };
struct EboChild : Empty { std::int64_t x; };        // 空基类优化
struct NoEbo : Empty { Empty e; };                  // 成员是 Empty 时不能优化（同地址不允许）
struct Mix1 { char c; std::int64_t i; };
struct Mix2 { std::int64_t i; char c; };            // 顺序不同大小不同
struct alignas(32) OverAligned { char c; };

struct Pod {
    char a;      // 偏移 0
    double d;    // 偏移 8（对齐 8）
    char b;      // 偏移 16
                 // 尾部填 7 → 24
};

int main() {
    std::println("── sizeof 数据表（arm64-apple-darwin）──");
    std::println("{:<22}{:>4}", "Empty", sizeof(Empty));            // 1：C++ 要求对象有唯一地址
    std::println("{:<22}{:>4}", "WithFunc(成员函数)", sizeof(WithFunc));
    std::println("{:<22}{:>4}", "WithVirt(+vptr)", sizeof(WithVirt));
    std::println("{:<22}{:>4}", "EboChild(:Empty)", sizeof(EboChild));  // EBO 吃掉空基类
    std::println("{:<22}{:>4}", "NoEbo(成员Empty)", sizeof(NoEbo));     // 两个 Empty 不能同地址
    std::println("{:<22}{:>4}", "Mix1{char,int64}", sizeof(Mix1));      // 1+7填+8
    std::println("{:<22}{:>4}", "Mix2{int64,char}", sizeof(Mix2));      // 8+1+7填（尾部填齐）
    std::println("{:<22}{:>4}", "Pod{c,d,b}", sizeof(Pod));
    std::println("{:<22}{:>4}", "OverAligned(alignas 32)", sizeof(OverAligned));

    std::println("\n── padding 证据：offsetof ──");
    std::println("a@{} d@{} b@{}  size={}（d 前填 {}，尾部填 {}）",
                 offsetof(Pod, a), offsetof(Pod, d), offsetof(Pod, b), sizeof(Pod),
                 offsetof(Pod, d) - offsetof(Pod, a) - 1,
                 sizeof(Pod) - offsetof(Pod, b) - 1);

    std::println("\n── variant vs union：类型安全 tag 要占位 ──");
    union Raw { std::int64_t i; double d; };
    std::variant<std::int64_t, double> v;
    std::println("union={} variant={}（variant 多出的空间 = 当前类型索引 + 对齐）",
                 sizeof(Raw), sizeof(v));
    v = 3.14;
    std::println("variant.index()={}（运行时可查，union 不行）", v.index());

    std::println("\n── 静态断言锁住布局假设（跨平台防回归）──");
    static_assert(sizeof(Empty) == 1);
    static_assert(sizeof(WithVirt) == 8);           // arm64 指针 8 字节
    static_assert(sizeof(Mix1) == 16 && sizeof(Mix2) == 16);
    static_assert(alignof(OverAligned) == 32);
    std::println("全部通过 —— 结构体布局变了，static_assert 先编译报错");

    // 对 new 的意义：Mix1 数组每元素都从 16 的倍数地址开始
    auto *arr = new Mix1[3];
    std::println("Mix1[0]@…{:x} Mix1[1]@…{:x}（相差 16）",
                 (std::uintptr_t)&arr[0] % 1000, (std::uintptr_t)&arr[1] % 1000);
    delete[] arr;
    return 0;
}
