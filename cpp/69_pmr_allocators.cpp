// 69 - <new> <memory_resource>：动态内存的底层设施（pmr 多态分配器）
#include <print>
#include <new>
#include <memory_resource>
#include <vector>
#include <list>
#include <string>

int main() {
    // ---- operator new 失败抛 bad_alloc；nothrow 版本返回 nullptr ----
    try {
        [[maybe_unused]] auto *p = new int[100];
        delete[] p;
    } catch (const std::bad_alloc &e) {
        std::println("bad_alloc: {}", e.what());
    }
    auto *q = new (std::nothrow) int[100];      // 不抛异常版本
    std::println("nothrow new 成功? {}", q != nullptr);
    delete[] q;

    // 对齐分配： aligned new（对齐要求超过默认时）
    auto *aligned = new (std::align_val_t{64}) int;   // 64 字节对齐
    std::println("对齐地址 64 字节? {}", reinterpret_cast<std::uintptr_t>(aligned) % 64 == 0);
    ::operator delete(aligned, std::align_val_t{64});

    // set_new_handler：分配失败时先调用的"最后手段"
    auto old = std::get_new_handler();
    std::set_new_handler([] { std::println("内存不足 handler（这里可以释放缓存再试）"); throw std::bad_alloc(); });
    std::set_new_handler(old);                  // 演示完恢复

    // ---- pmr：运行期可换的"多态分配器" ----
    // monotonic_buffer_resource：栈上缓冲，只分配不回收，最省事最快
    std::byte buf[1024];
    std::pmr::monotonic_buffer_resource pool{buf, sizeof buf};

    std::pmr::vector<int> pv{&pool};            // 这个 vector 的内存全部来自 buf
    for (int i = 0; i < 50; i++) pv.push_back(i);
    std::println("pmr vector: size={} 第一个={} 最后一个={}", pv.size(), pv.front(), pv.back());
    // pool 析构一次性释放，pv 不用管

    // unsynchronized_pool_resource：池化分配，适合大量小对象
    std::pmr::unsynchronized_pool_resource object_pool;
    std::pmr::vector<std::string> sv{&object_pool};
    sv.emplace_back("hello pmr");
    sv.emplace_back("第二个字符串");
    std::println("pmr string vector: {} / {}", sv[0], sv[1]);

    // 不同容器共用同一资源
    std::pmr::list<int> pl{&object_pool};
    pl.push_back(1);
    std::println("pmr list size={}", pl.size());

    // new_delete_resource：默认的全局堆资源；null_memory_resource：总是失败（测试用）
    std::pmr::vector<int> heap_vec{std::pmr::new_delete_resource()};
    heap_vec.push_back(42);
    std::println("new_delete_resource 容器: {}", heap_vec[0]);
    return 0;
}
