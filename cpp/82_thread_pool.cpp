// 82 - 进阶：手写线程池 —— fixed-size pool + future 任务句柄
#include <print>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <vector>
#include <chrono>

class ThreadPool {
public:
    explicit ThreadPool(unsigned n) {
        for (unsigned i = 0; i < n; i++)
            workers_.emplace_back([this] { worker_loop(); });
    }
    ~ThreadPool() {
        {
            std::lock_guard lock(m_);
            stop_ = true;
        }
        cv_.notify_all();                    // 唤醒所有 worker 让它们看到 stop
    }

    // submit 返回 future：任务结果/异常都能取回
    template <typename F>
    auto submit(F f) -> std::future<std::invoke_result_t<F>> {
        using R = std::invoke_result_t<F>;
        auto task = std::make_shared<std::packaged_task<R()>>(std::move(f));
        std::future<R> fut = task->get_future();
        {
            std::lock_guard lock(m_);
            tasks_.emplace([task] { (*task)(); });   // 擦掉类型，队列里只存 function
        }
        cv_.notify_one();
        return fut;
    }

private:
    void worker_loop() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock lock(m_);
                cv_.wait(lock, [&] { return stop_ || !tasks_.empty(); });  // 带谓词防虚假唤醒
                if (stop_ && tasks_.empty()) return;   // 优雅退出：排空队列
                job = std::move(tasks_.front());
                tasks_.pop();
            }
            job();                                     // 锁外执行，不阻塞队列
        }
    }

    std::vector<std::jthread> workers_;    // jthread：析构自动 join
    std::queue<std::function<void()>> tasks_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stop_ = false;
};

static long long heavy_sum(int n) {        // 模拟 CPU 密集任务
    long long s = 0;
    for (int i = 0; i < n; i++) s += i % 7;
    return s;
}

int main() {
    const unsigned HW = std::thread::hardware_concurrency();
    std::println("硬件线程数 = {}", HW);

    // ---- 串行 vs 线程池：同样的 8 个任务 ----
    constexpr int TASKS = 8;
    auto t0 = std::chrono::steady_clock::now();
    long long checksum = 0;
    for (int i = 0; i < TASKS; i++) checksum += heavy_sum(20'000'000);
    auto t1 = std::chrono::steady_clock::now();

    {
        ThreadPool pool(HW);
        std::vector<std::future<long long>> futs;
        auto t2 = std::chrono::steady_clock::now();
        for (int i = 0; i < TASKS; i++)
            futs.push_back(pool.submit([] { return heavy_sum(20'000'000); }));
        long long pool_sum = 0;
        for (auto &f : futs) pool_sum += f.get();   // get 阻塞到对应任务完成
        auto t3 = std::chrono::steady_clock::now();
        auto ms = [](auto d) { return std::chrono::duration<double, std::milli>(d).count(); };
        std::println("串行: {:>7.1f} ms  校验={}", ms(t1 - t0), checksum);
        std::println("线程池({} 线程): {:>5.1f} ms  校验={}（应相等）", HW, ms(t3 - t2), pool_sum);
        std::println("加速比 ≈ {:.1f}x（8 任务 ÷ {} 线程，受任务不均与调度影响）",
                     ms(t1 - t0) / ms(t3 - t2), HW);
    }                                                // 析构：通知 + join，全部任务已排空

    // 异常也能穿透线程池
    {
        ThreadPool pool(2);
        auto f = pool.submit([]() -> int { throw std::runtime_error("任务内出错"); });
        try {
            (void)f.get();
        } catch (const std::exception &e) {
            std::println("从 future 取到异常: {}", e.what());
        }
    }
    std::println("要点：队列 + 互斥锁 + 条件变量 + packaged_task 就是线程池的全部秘密");
    return 0;
}
