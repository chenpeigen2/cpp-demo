// 30 - Hello C++23：iostream 与命名空间
#include <iostream>
#include <print>

int main() {
    std::cout << "hello, C++23!\n";            // 流式输出，运算符 << 重载
    int x = 42;
    double pi = 3.14159;
    std::cout << "x=" << x << " pi=" << pi << '\n';

    // C++23 的 <print>：类型安全的格式化输出，比 iostream 简洁
    std::println("x={} pi={:.2f}", x, pi);     // {} 占位，{:.2f} 保留两位
    std::print("no newline");

    // 用户输入（注释掉以免阻塞自动化运行）：
    // std::cin >> x;

    // std::cerr 立即刷新（错误）、std::clog 带缓冲（日志）
    std::cerr << "(stderr)\n";
    std::println("");
    return 0;
}
