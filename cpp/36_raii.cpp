// 36 - RAII：资源获取即初始化，C++ 最重要的惯用法
#include <print>
#include <fstream>
#include <stdexcept>
#include <mutex>

class Logger {                                 // 用析构自动收尾
    std::ofstream &f_;
public:
    explicit Logger(std::ofstream &f) : f_(f) { f_ << "[开始]\n"; }
    ~Logger() { f_ << "[结束]\n"; }            // 无论怎么退出作用域都会执行
};

int risky(int v) {
    if (v < 0) throw std::invalid_argument("负数");   // 异常沿调用栈向上传播
    return 100 / v;
}

int main() {
    {
        std::ofstream file("/tmp/cpp-demo-raii.txt");
        Logger log(file);                      // 构造即"获取"，析构即"释放"
        file << "工作中...\n";
        try {
            risky(-1);                         // 抛异常 → 栈展开 → log/file 依次析构
        } catch (const std::exception &e) {    // 按引用捕获（避免切片）
            std::println("捕获: {}", e.what());
        }
    }                                          // 即使没异常，出块也自动关文件

    // 标准库的 RAII 工具：lock_guard 管锁、unique_ptr 管内存
    std::mutex m;
    {
        std::lock_guard lock(m);               // 构造加锁，析构解锁
        std::println("临界区内");
    }                                          // 这里自动解锁，忘不掉

    // 栈展开验证：RAII 保证资源不泄漏
    std::println("文件内容：");
    std::ifstream in("/tmp/cpp-demo-raii.txt");
    std::string line;
    while (std::getline(in, line)) std::println("  {}", line);
    return 0;
}
