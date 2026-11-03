// 51 - <memory>：智能指针
#include <print>
#include <memory>
#include <vector>

struct Task {
    std::string name;
    Task(std::string n) : name(std::move(n)) { std::println("[构造] {}", name); }
    ~Task() { std::println("[析构] {}", name); }
    void run() { std::println("{} 运行中", name); }
};

// 自定义删除器：智能指针也能管 FILE* 等非 new 资源
struct FileCloser {
    void operator()(std::FILE *f) const { if (f) { std::println("自定义删除器关文件"); fclose(f); } }
};
using FilePtr = std::unique_ptr<std::FILE, FileCloser>;

int main() {
    // unique_ptr：独占所有权，零开销，不可拷贝只可移动
    auto u = std::make_unique<Task>("独占任务");
    u->run();
    auto u2 = std::move(u);                  // 转移所有权后 u == nullptr
    std::println("u 已空? {}", u == nullptr);

    // unique_ptr 管数组
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    std::println("arr[0]={}", arr[0]);

    // shared_ptr：引用计数共享，最后一个释放
    auto s = std::make_shared<Task>("共享任务");
    std::println("use_count={}", s.use_count());
    {
        auto s2 = s;
        std::println("块内 use_count={}", s.use_count());
    }
    std::println("块外 use_count={}", s.use_count());

    // weak_ptr：观察但不持有，解决循环引用
    std::weak_ptr<Task> w = s;
    std::println("weak 过期? use_count={}", w.expired(), s.use_count());
    if (auto locked = w.lock())              // 要用时 lock 提升成 shared_ptr
        locked->run();
    s.reset();
    std::println("reset 后 weak 过期? {}", w.expired());

    // 自定义删除器
    {
        FilePtr f(std::fopen("/tmp/cpp-demo-sp.txt", "w"));
        std::fputs("hi", f.get());
    }                                        // 出作用域自动调用 FileCloser

    // 裸 new/delete 仍可用，但现代 C++ 尽量避免
    auto *raw = new Task{"裸分配"};
    delete raw;
    return 0;
}
