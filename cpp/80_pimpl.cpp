// 80 - 进阶：Pimpl 惯用法 —— 编译防火墙与稳定 ABI
#include <print>
#include <memory>
#include <string>
#include <map>
#include <chrono>

// ---- 头文件里只留一个指针：实现细节全部藏进 .cpp ----
class Database {
public:
    Database();                                          // 构造在 .cpp（能见到 Impl）
    ~Database();                                         // 析构必须显式声明（Impl 不完整类型）
    Database(Database &&) noexcept;                      // 移动支持
    Database &operator=(Database &&) noexcept;

    void put(const std::string &key, int value);
    int  get(const std::string &key) const;
    std::size_t size() const;

private:
    struct Impl;                                         // 前置声明
    std::unique_ptr<Impl> impl_;                         // 唯一成员：一个指针
};

// ---- "cpp 文件"：真实实现 ----
struct Database::Impl {
    std::map<std::string, int> storage;
    int lookups = 0;
};

Database::Database() : impl_(std::make_unique<Impl>()) {}
Database::~Database() = default;                         // unique_ptr<Impl> 需要完整类型 → 放这里
Database::Database(Database &&) noexcept = default;
Database &Database::operator=(Database &&) noexcept = default;

void Database::put(const std::string &k, int v) { impl_->storage[k] = v; }
int Database::get(const std::string &k) const {
    const_cast<Impl &>(*impl_).lookups++;
    return impl_->storage.contains(k) ? impl_->storage.at(k) : -1;
}
std::size_t Database::size() const { return impl_->storage.size(); }

int main() {
    Database db;
    db.put("a", 1);
    db.put("b", 2);
    std::println("size={} get(a)={} get(x)={}", db.size(), db.get("a"), db.get("x"));

    Database moved = std::move(db);                      // Pimpl 对象移动 = 移动一个指针
    std::println("移动后 get(b)={}", moved.get("b"));

    // Pimpl 的三大收益：
    std::println("\n1) 编译防火墙：std::map 等重头文件只出现在实现里，改动实现不触发用户重编译");
    std::println("   sizeof(Database)={} —— 头文件不含任何 STL，二进制接口极小", sizeof(Database));

    // 2) 稳定 ABI：成员布局不随实现变化，动态库升级不破坏链接
    // 3) 异常安全边界 + 延迟分配
    auto t0 = std::chrono::steady_clock::now();
    Database lazy;
    auto t1 = std::chrono::steady_clock::now();
    std::println("2) 构造耗时 {:.1f}ns（实际做了一次 make_unique+map）",
                 std::chrono::duration<double, std::nano>(t1 - t0).count());
    std::println("3) 代价：每次访问多一层指针间接 + 堆分配；读多写少的热数据别用 Pimpl");

    // 现代替代：scoped enum + unique_ptr 已把 Pimpl 简化到 5 行；
    // 更大的收益来自"接口隔离"——头文件里不该出现的东西都进 Impl
    return 0;
}
