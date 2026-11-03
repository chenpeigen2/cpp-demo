// 66 - <atomic>：原子类型与内存序（C++ 版本比 C 的更面向对象）
#include <print>
#include <atomic>
#include <thread>
#include <vector>

int main() {
    // 原子计数：多线程 fetch_add 无数据竞争
    std::atomic<int> counter{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; i++)
        pool.emplace_back([&] {
            for (int j = 0; j < 25000; j++)
                counter.fetch_add(1, std::memory_order_relaxed);  // 无需强序时用 relaxed
        });
    for (auto &t : pool) t.join();
    std::println("atomic counter={}", counter.load());

    // load / store / exchange
    std::atomic<int> x{1};
    x.store(2);
    int old = x.exchange(3);
    std::println("exchange: 旧={} 新={}", old, x.load());

    // CAS：比较交换，无锁编程的核心
    int expected = 3;
    bool ok = x.compare_exchange_strong(expected, 5);
    std::println("CAS(3→5): ok={} now={} expected={}", ok, x.load(), expected);

    // 原子标志
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();
    std::println("atomic_flag test={}", flag.test());

    // 原子指针与 is_lock_free
    int value = 7;
    std::atomic<int *> ap{&value};
    std::println("atomic pointer 指向 *{}  lock_free={}", *ap.load(), std::atomic<int>{}.is_lock_free());

    // release/acquire 建立同步：写线程 release，读线程 acquire 看到全部写入
    std::atomic<int> data{0}, ready{0};
    std::thread producer([&] {
        data.store(42, std::memory_order_relaxed);
        ready.store(1, std::memory_order_release);      // 发布：之前的写入对 acquire 方可见
    });
    std::thread consumer([&] {
        while (ready.load(std::memory_order_acquire) == 0) {}   // 订阅
        std::println("acquire 读到 data={}", data.load());      // 保证 42
    });
    producer.join();
    consumer.join();

    // atomic_ref (C++20)：把普通对象临时当原子用
    int plain = 0;
    {
        std::atomic_ref<int> r(plain);
        r.fetch_add(5);
    }
    std::println("atomic_ref 后 plain={}", plain);
    return 0;
}
