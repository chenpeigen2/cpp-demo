// 61 - <coroutine>：协程（C++20，标准库只给"零件"，任务类型自己组装）
#include <print>
#include <coroutine>
#include <iterator>
#include <exception>
#include <utility>
#include <string>

// ---- 生成器：co_yield 的载体，每次取一个值，惰性产生 ----
template <typename T>
class Generator {
public:
    struct promise_type {                      // 编译器通过它控制协程行为
        T current_{};
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }  // 创建即挂起（惰性）
        std::suspend_always final_suspend() noexcept { return {}; }    // 结束也挂起，由析构销毁
        std::suspend_always yield_value(T v) { current_ = v; return {}; }  // co_yield 落点
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    explicit Generator(std::coroutine_handle<promise_type> h) : h_(h) {}
    Generator(Generator &&o) noexcept : h_(std::exchange(o.h_, {})) {}
    Generator(const Generator &) = delete;     // 协程句柄不可拷贝
    ~Generator() { if (h_) h_.destroy(); }

    struct iterator {                          // 让生成器支持 range-for
        std::coroutine_handle<promise_type> h;
        bool done = false;
        iterator &operator++() { h.resume(); done = h.done(); return *this; }
        const T &operator*() const { return h.promise().current_; }
        bool operator!=(std::default_sentinel_t) const { return !done; }
    };
    iterator begin() { h_.resume(); return {h_, h_.done()}; }
    std::default_sentinel_t end() { return {}; }

private:
    std::coroutine_handle<promise_type> h_;    // 协程帧的句柄
};

// 无限斐波那契：调用一次取一个，不预计算
Generator<int> fib() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;                            // 挂起并交出 a，下次 resume 从这继续
        auto t = a + b;
        a = b;
        b = t;
    }
}

// ---- 惰性任务：co_return 的载体，创建不执行，取值才算 ----
template <typename T>
class Lazy {
public:
    struct promise_type {
        T value{};
        Lazy get_return_object() {
            return Lazy{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { value = v; }  // co_return 落点
        void unhandled_exception() { std::terminate(); }
    };
    explicit Lazy(std::coroutine_handle<promise_type> h) : h_(h) {}
    Lazy(Lazy &&o) noexcept : h_(std::exchange(o.h_, {})) {}
    Lazy(const Lazy &) = delete;
    ~Lazy() { if (h_) h_.destroy(); }

    T get() {
        if (!h_.done()) h_.resume();           // 首次取值才真正执行
        return h_.promise().value;
    }
private:
    std::coroutine_handle<promise_type> h_;
};

// co_await 演示：就绪型 awaiter（await_ready=true 不挂起，直接取值）
struct ReadyAwaiter {
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const noexcept { return 7; }   // co_await 表达式的值
};

Lazy<int> compute() {
    int base = co_await ReadyAwaiter{};        // co_await：等待/获取异步结果
    co_return base * 6;                        // 协程的返回值
}

int main() {
    std::print("斐波那契(前9个): ");
    int count = 0;
    for (int x : fib()) {
        std::print("{} ", x);
        if (++count == 9) break;
    }
    std::println("");

    Lazy<int> task = compute();
    std::println("惰性任务结果={}", task.get());
    return 0;
}
