// 33 - 特殊成员函数：拷贝/移动构造、拷贝/移动赋值（三五零法则）
#include <print>
#include <utility>
#include <cstring>

class Buffer {
    char *data_ = nullptr;
    std::size_t size_ = 0;
public:
    explicit Buffer(std::size_t n) : data_(new char[n]), size_(n) {
        std::println("构造 {} 字节", n);
    }
    ~Buffer() { delete[] data_; }                          // 析构释放资源

    Buffer(const Buffer &o)                                // 拷贝构造：深拷贝
        : data_(new char[o.size_]), size_(o.size_) {
        std::memcpy(data_, o.data_, size_);
        std::println("拷贝构造 {} 字节", size_);
    }
    Buffer &operator=(const Buffer &o) {                   // 拷贝赋值
        if (this == &o) return *this;                      // 自赋值保护
        char *nd = new char[o.size_];
        std::memcpy(nd, o.data_, o.size_);
        delete[] data_;
        data_ = nd; size_ = o.size_;
        std::println("拷贝赋值");
        return *this;
    }
    Buffer(Buffer &&o) noexcept                            // 移动构造：窃取资源
        : data_(std::exchange(o.data_, nullptr)), size_(std::exchange(o.size_, 0)) {
        std::println("移动构造");
    }
    Buffer &operator=(Buffer &&o) noexcept {               // 移动赋值
        if (this == &o) return *this;
        delete[] data_;
        data_ = std::exchange(o.data_, nullptr);
        size_ = std::exchange(o.size_, 0);
        std::println("移动赋值");
        return *this;
    }
    std::size_t size() const { return size_; }
};

int main() {
    Buffer a{100};
    Buffer b = a;                 // 拷贝构造
    Buffer c = std::move(a);      // 移动构造：a 之后处于"有效但未指定"状态
    b = c;                        // 拷贝赋值
    b = std::move(c);             // 移动赋值
    std::println("a={} b={} c={}", a.size(), b.size(), c.size());
    return 0;
}
