// 67 - <semaphore> <latch> <barrier>：C++20 协调原语
#include <print>
#include <semaphore>
#include <latch>
#include <barrier>
#include <mutex>
#include <thread>
#include <vector>

std::counting_semaphore<2> slots(2);        // 资源池：最多 2 个并发
std::mutex print_mutex;

static void worker(int id) {
    slots.acquire();                        // 拿一个"名额"，没有就阻塞
    {
        std::lock_guard l(print_mutex);
        std::println("worker {} 进入（同时最多2个）", id);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    slots.release();                        // 归还名额
}

int main() {
    // ---- counting_semaphore：限制并发数（信号量）----
    std::vector<std::jthread> pool;
    for (int i = 1; i <= 5; i++) pool.emplace_back(worker, i);
    for (auto &t : pool) t.join();
    std::println("信号量演示完毕");

    // ---- binary_semaphore：0/1 信号量，可当"一次性事件"用 ----
    std::binary_semaphore event(0);         // 初始无信号
    std::jthread waiter([&] {
        event.acquire();                    // 等待通知
        std::println("收到信号，继续执行");
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    event.release();                        // 发出通知
    waiter.join();

    // ---- latch：一次性倒计时门闩，减到 0 打开 ----
    std::latch start_gate(1);               // 主线程发令枪
    std::latch done_gate(3);                // 3 个线程各自报告
    std::vector<std::jthread> runners;
    for (int i = 1; i <= 3; i++)
        runners.emplace_back([&, i] {
            start_gate.wait();              // 等发令
            {
                std::lock_guard l(print_mutex);
                std::println("runner {} 起跑", i);
            }
            done_gate.count_down();         // 报告完成
        });
    start_gate.count_down();                // 发令
    done_gate.wait();                       // 等全部完成
    for (auto &t : runners) t.join();
    std::println("latch 演示完毕");

    // ---- barrier：可重复使用的同步点（多阶段计算）----
    std::barrier sync(2);                   // 2 人一组，到齐放行
    std::jthread phase_worker([&] {
        for (int phase = 1; phase <= 2; phase++) {
            {
                std::lock_guard l(print_mutex);
                std::println("阶段 {} 工作中", phase);
            }
            sync.arrive_and_wait();         // 等主线程一起进入下一阶段
        }
    });
    for (int phase = 1; phase <= 2; phase++) {
        sync.arrive_and_wait();
        {
            std::lock_guard l(print_mutex);
            std::println("主线程同步完成阶段 {}", phase);
        }
    }
    phase_worker.join();
    return 0;
}
