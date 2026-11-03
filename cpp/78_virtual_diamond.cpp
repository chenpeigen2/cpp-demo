// 78 - 进阶：虚函数的代价与菱形继承 —— 数据看虚表
#include <print>
#include <memory>
#include <vector>
#include <chrono>

// ---- 菱形继承问题 ----
struct Device {
    int id = 1;
    virtual ~Device() = default;
    virtual void boot() { std::println("  Device::boot id={}", id); }
};
struct Radio : virtual Device {          // virtual 继承：共享基类子对象
    void boot() override { std::println("  Radio::boot"); }
};
struct Screen : virtual Device {
    void boot() override { std::println("  Screen::boot"); }
};
struct Phone final : Radio, Screen {     // 菱形顶点
    void boot() override { std::println("  Phone::boot（Radio/Screen 的 boot 都被覆盖）"); }
};

// sizeof 数据表：虚继承的指针代价
struct PlainBase { int x; };
struct VBase    { virtual double area() const = 0; virtual ~VBase() = default; int x; };
struct VDerived : VBase { int y; };
struct VV1 : virtual PlainBase { int a; };
struct VV2 : virtual PlainBase { int b; };
struct Diamond : VV1, VV2 {};

int main() {
    std::println("── 内存布局数据 ──");
    std::println("PlainBase(无虚) ={} 字节", sizeof(PlainBase));
    std::println("VBase(带虚函数) ={} 字节（多出的 8 是 vptr）", sizeof(VBase));
    std::println("VDerived        ={} 字节（继承 vptr，不新增）", sizeof(VDerived));
    std::println("VV1/virtual继承  ={} 字节（+vbptr 指向虚基偏移表）", sizeof(VV1));
    std::println("Diamond 双重虚继承 = {} 字节（两份 vbptr + 共享一份 Base）", sizeof(Diamond));

    std::println("\n── 菱形继承行为 ──");
    Phone p;
    Device &d = p;              // 虚继承保证只有一份 Device 子对象
    d.id = 99;
    d.boot();                   // 调到 Phone::boot（最派生的覆盖）
    std::println("  Phone 大小={}（含 Radio、Screen、共享的 Device）", sizeof(Phone));

    // 没有 virtual 继承会怎样：两份 Device 子对象，d.id 二义编译错误
    // （把上面两个 virtual 去掉试试：error: member 'id' found in multiple base classes）

    std::println("\n── 虚调用开销实测（间接跳转 vs 直接调用）──");
    struct Impl final : VBase {
        double area_direct() const { return 2; }          // 非虚版本（可内联）
        double area() const override { return 2; }        // 虚版本
    };
    Impl impl;
    VBase &base = impl;
    constexpr long N = 100'000'000;
    auto t0 = std::chrono::steady_clock::now();
    volatile double s1 = 0;
    for (long i = 0; i < N; i++) s1 += impl.area_direct();    // 直接（可内联）
    auto t1 = std::chrono::steady_clock::now();
    volatile double s2 = 0;
    for (long i = 0; i < N; i++) s2 += base.area();           // 虚分派
    auto t2 = std::chrono::steady_clock::now();
    auto us = [](auto x) { return std::chrono::duration<double, std::micro>(x).count(); };
    std::println("  直接调用 {:>8.0f} µs   虚调用 {:>8.0f} µs   （差 ≈ {} ns/次）",
                 us(t1 - t0), us(t2 - t1), (us(t2 - t1) - us(t1 - t0)) * 1000 / N);

    // 多态容器的真实成本：每个元素独立堆分配（缓存不友好）
    std::vector<std::unique_ptr<VBase>> objs;
    objs.reserve(1000);
    for (int i = 0; i < 1000; i++) objs.push_back(std::make_unique<Impl>());
    std::println("  1000 个多态对象 = {} 字节（{} 堆分配，遍历有指针跳转）",
                 objs.size() * sizeof(Impl) + objs.capacity() * 8, objs.size());
    std::println("经验法则：热路径大批量同质数据 → 结构体数组 + CRTP/variant；真正异构才用虚");
    return 0;
}
