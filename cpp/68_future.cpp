// 68 - <future>：异步结果（async / promise / packaged_task）
#include <print>
#include <future>
#include <thread>
#include <chrono>

static int slow_add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    return a + b;
}

int main() {
    // ---- std::async：把函数扔到别的线程，拿 future 收结果 ----
    std::future<int> f = std::async(std::launch::async, slow_add, 2, 3);
    std::println("主线程可以先干别的……");
    std::println("async 结果={}", f.get());          // get 阻塞直到就绪（只能调一次）

    // deferred：惰性，get 时才在当前线程执行
    std::future<int> lazy = std::async(std::launch::deferred, [] { return 99; });
    std::println("deferred 状态={}", lazy.wait_for(std::chrono::seconds(0)) == std::future_status::deferred ? "未启动" : "?");
    std::println("deferred get={}", lazy.get());

    // future 状态查询
    std::future<int> pending = std::async(std::launch::async, slow_add, 1, 1);
    std::println("pending ready? {}",
                 pending.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready);
    pending.wait();
    std::println("wait 后 ready={} 值={}",
                 pending.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready, 2);

    // ---- promise/future 对：手动传递值或异常 ----
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread setter([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        prom.set_value(777);                // 也可以 set_exception 传异常
    });
    std::println("promise 传值={}", fut.get());
    setter.join();

    // 异常跨线程传播：set_exception → get 时重抛
    std::promise<int> perr;
    std::future<int> ferr = perr.get_future();
    std::thread thrower([&] {
        perr.set_exception(std::make_exception_ptr(std::runtime_error("线程内出错")));
    });
    try {
        int v = ferr.get();
        std::println("不应该到这: {}", v);
    } catch (const std::exception &e) {
        std::println("跨线程异常: {}", e.what());
    }
    thrower.join();

    // ---- packaged_task：把可调用物包装成异步任务，future 由它管 ----
    std::packaged_task<int(int, int)> task(slow_add);
    std::future<int> tf = task.get_future();
    std::thread runner(std::move(task), 10, 20);    // 任务可在任意线程执行
    std::println("packaged_task 结果={}", tf.get());
    runner.join();

    // shared_future：可多线程多次 get
    std::shared_future<int> sf = std::async(std::launch::async, [] { return 5; }).share();
    std::println("shared_future={} {}", sf.get(), sf.get());   // 可重复取
    return 0;
}
