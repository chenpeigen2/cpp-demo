// 32 - 类与封装：成员、构造/析构、this
#include <print>
#include <string>

class Counter {                    // class 默认 private，struct 默认 public
    std::string name_;
    int count_ = 0;                // 类内默认初始化（C++11）
public:
    Counter(std::string name, int start = 0)   // 构造函数（可带默认参数）
        : name_(std::move(name)), count_(start) {}  // 成员初始化列表

    ~Counter() { std::println("[析构] {} 最终 {}", name_, count_); }

    Counter &inc() { ++count_; return *this; }   // 返回引用支持链式调用
    int get() const {              // const 成员函数：承诺不修改对象
        return count_;
    }
    const std::string &name() const { return name_; }

    void reset() { count_ = 0; }   // this 是指向自身的指针
    Counter &chain() { ++count_; return *this; }  // 返回 *this 支持链式调用
};

int main() {
    Counter c{"计数器", 10};
    c.inc().inc().chain();         // 链式调用
    std::println("{} = {}", c.name(), c.get());

    Counter d{"另一个"};            // 默认参数 start=0
    d.inc();
    std::println("{} = {}", d.name(), d.get());
    return 0;                      // 出作用域按构造的逆序析构
}
