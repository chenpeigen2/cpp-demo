// 35 - 继承与虚函数：动态多态
#include <print>
#include <memory>
#include <vector>

class Animal {
public:
    Animal() = default;
    virtual ~Animal() = default;             // 有虚函数的基类必须有虚析构
    virtual void speak() const { std::println("..."); }
    virtual std::string name() const = 0;    // 纯虚函数 → 抽象类，不能实例化
};

class Dog final : public Animal {            // final：禁止再被继承
public:
    void speak() const override { std::println("汪汪"); }
    std::string name() const override { return "狗"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::println("喵"); }
    std::string name() const override { return "猫"; }
};

int main() {
    // 通过基类引用/指针调用虚函数 → 运行期按实际类型分派
    Dog d; Cat c;
    std::vector<std::unique_ptr<Animal>> zoo;
    zoo.push_back(std::make_unique<Dog>());
    zoo.push_back(std::make_unique<Cat>());
    for (const auto &a : zoo)
        a->speak();                          // 虚表(vtable)查找

    Animal &ra = d;                          // 基类引用指向派生类对象
    std::println("{}说：", ra.name());
    ra.speak();

    // dynamic_cast：安全的向下转型（RTTI），失败返回 nullptr
    if (Dog *pd = dynamic_cast<Dog *>(zoo[0].get()))
        std::println("确认是{}", pd->name());
    if (dynamic_cast<Dog *>(zoo[1].get()) == nullptr)
        std::println("猫不是狗");
    return 0;
}
