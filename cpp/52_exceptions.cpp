// 52 - <stdexcept> <exception>：异常体系
#include <print>
#include <stdexcept>
#include <exception>

// 自定义异常：继承 std::exception，覆写 what()
class NetworkError : public std::runtime_error {
public:
    explicit NetworkError(const std::string &msg) : std::runtime_error(msg) {}
};

// noexcept 承诺不抛异常；违约会导致 std::terminate
int fast_add(int a, int b) noexcept { return a + b; }

static void level3() { throw NetworkError("连接超时"); }
static void level2() { level3(); }
static void level1() { level2(); }           // 异常沿调用栈向上"栈展开"

int main() {
    // 标准异常层次（都派生自 std::exception）
    // logic_error:    invalid_argument / out_of_range / length_error
    // runtime_error:  range_error / overflow_error / system_error
    try {
        throw std::out_of_range("下标越界");
    } catch (const std::out_of_range &e) {       // 精确类型先匹配
        std::println("捕获 out_of_range: {}", e.what());
    } catch (const std::exception &e) {          // 基类兜底（放后面）
        std::println("捕获 exception: {}", e.what());
    }

    try {
        level1();
    } catch (const NetworkError &e) {
        std::println("捕获自定义异常: {}", e.what());
    }

    // 重新抛出与多级捕获
    try {
        try {
            throw std::invalid_argument("参数为空");
        } catch (...) {                          // catch(...) 接住一切
            std::println("内层先记录日志");
            throw;                               // 裸 throw 重新抛出当前异常
        }
    } catch (const std::exception &e) {
        std::println("外层拿到: {}", e.what());
    }

    // std::current_exception / exception_ptr：把异常当值保存传递
    std::exception_ptr saved;
    try {
        throw std::runtime_error("存起来的异常");
    } catch (...) { saved = std::current_exception(); }
    try {
        if (saved) std::rethrow_exception(saved);
    } catch (const std::exception &e) {
        std::println("延迟重抛: {}", e.what());
    }

    // uncaught_exceptions：当前未处理异常数（比 bool 版本多一层语义）
    std::println("uncaught={} noexcept 函数={}", std::uncaught_exceptions(), fast_add(1, 2));
    return 0;
}
