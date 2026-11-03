// 60 - <thread> <mutex> <shared_mutex> <condition_variable> <stop_token>
#include <print>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <stop_token>
#include <queue>
#include <vector>

static std::mutex io_mutex;
static int shared_counter = 0;
static std::mutex counter_mutex;

static void say(int id) {                    // 多线程输出：用锁保护避免交错
    std::lock_guard lock(io_mutex);
    std::println("线程 {} 说：整行输出不被打断", id);
}

static void increment(int times) {
    for (int i = 0; i < times; i++) {
        std::lock_guard lock(counter_mutex); // RAII 锁：构造加锁析构解锁
        shared_counter++;
    }
}

// stop_token (C++20)：协作式取消线程的标准机制
static void cancellable_worker(std::stop_token st) {
    int rounds = 0;
    while (!st.stop_requested()) {           // 外部请求停止时退出
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        rounds++;
    }
    std::lock_guard lock(io_mutex);
    std::println("协作线程收到停止信号，循环了 {} 轮", rounds);
}

int main() {
    std::println("硬件线程数={}", std::thread::hardware_concurrency());

    // jthread (C++20)：析构时自动 request_stop + join，不会忘记收尾
    {
        std::jthread jt([] { say(0); });
        std::println("jthread joinable? {}", jt.joinable());
    }                                        // 出作用域自动 join

    // 多线程互斥计数
    std::vector<std::thread> pool;
    for (int i = 1; i <= 4; i++) pool.emplace_back(increment, 25000);
    for (auto &th : pool) th.join();
    std::println("counter={}（期望 100000）", shared_counter);

    // 读写锁：shared_mutex 允许多读单写
    std::shared_mutex rw;
    {
        std::shared_lock rlock(rw);          // 读锁（共享，可多个线程同时持有）
        std::println("读锁持有中");
    }
    {
        std::unique_lock wlock(rw);          // 写锁（独占）
        std::println("写锁持有中");
    }

    // 多把锁：scoped_lock 一次性获取，内部用避免死锁的算法
    std::mutex m1, m2;
    {
        std::scoped_lock both(m1, m2);
        std::println("scoped_lock 两把锁");
    }

    // 条件变量：生产者-消费者
    std::queue<int> q;
    std::mutex qm;
    std::condition_variable cv;
    bool done = false;

    std::thread consumer([&] {
        std::unique_lock lock(qm);
        cv.wait(lock, [&] { return !q.empty() || done; });  // 带谓词等待（防虚假唤醒）
        if (!q.empty()) {
            std::lock_guard l(io_mutex);
            std::println("消费者收到 {}", q.front());
        }
    });
    {
        std::lock_guard lock(qm);
        q.push(42);
    }
    cv.notify_one();
    consumer.join();

    // once_flag：多线程只执行一次的初始化
    static std::once_flag flag;
    std::vector<std::thread> once_pool;
    for (int i = 0; i < 3; i++)
        once_pool.emplace_back([] {
            std::call_once(flag, [] { std::println("call_once 只打印一次"); });
        });
    for (auto &th : once_pool) th.join();

    // stop_source/stop_token：主动请求子线程停止
    {
        std::jthread worker(cancellable_worker);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        worker.request_stop();               // 设置停止标志
    }                                        // 自动 join
    std::println("注：std::osyncstream(<syncstream>) 在 Apple libc++ 尚未提供，"
                 "用互斥锁保护输出演示同一问题");
    return 0;
}
