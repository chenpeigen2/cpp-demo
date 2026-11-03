// 59 - <filesystem>：C++17 标准文件系统
#include <print>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

int main() {
    const fs::path dir = "/tmp/cpp-demo-fs";

    // 目录操作
    fs::create_directories(dir / "sub/inner");   // 递归创建
    std::println("当前路径: {}", fs::current_path().string());
    std::println("存在? {} 是目录? {}", fs::exists(dir), fs::is_directory(dir));

    // 写几个文件
    for (const char *name : {"a.txt", "b.log"}) {
        std::ofstream f(dir / name);
        if (f) f << "content of " << name;
    }

    // path 的成员函数：路径就是对象
    fs::path p = dir / "a.txt";
    std::println("path: parent={} filename={} stem={} ext={}",
                 p.parent_path().string(), p.filename().string(),
                 p.stem().string(), p.extension().string());

    // 文件信息
    std::println("大小={} 字节", fs::file_size(p));
    std::println("绝对路径: {}", fs::absolute(p).string());

    // 遍历目录：directory_iterator 单层；recursive_directory_iterator 递归
    std::print("目录树: ");
    for (const auto &entry : fs::recursive_directory_iterator(dir))
        std::print("{} ", entry.path().filename().string());
    std::println("");

    // 复制/重命名/删除
    fs::copy_file(p, dir / "copy.txt", fs::copy_options::overwrite_existing);
    fs::rename(dir / "copy.txt", dir / "renamed.txt");
    std::println("renamed 存在? {}", fs::exists(dir / "renamed.txt"));

    // 权限
    fs::permissions(dir / "b.log", fs::perms::owner_write, fs::perm_options::remove);
    std::println("b.log 只读? {}", (fs::status(dir / "b.log").permissions() & fs::perms::owner_write) == fs::perms::none);
    fs::permissions(dir / "b.log", fs::perms::owner_write, fs::perm_options::add);

    // 空间信息与其他
    auto space = fs::space("/");
    std::println("/ 剩余 {} GB", space.free / (1u << 30));

    // 清理
    fs::remove_all(dir);
    std::println("清理后存在? {}", fs::exists(dir));
    return 0;
}
