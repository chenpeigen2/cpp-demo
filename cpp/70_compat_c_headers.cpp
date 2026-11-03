// 70 - C 兼容头（<cXXX>）：C++ 里的 C 标准库门面
#include <print>
#include <cstddef>     // size_t ptrdiff_t nullptr_t byte
#include <cstdint>     // 定宽整数（C++ 版）
#include <climits>     // 整数极限
#include <cfloat>      // 浮点极限
#include <csetjmp>     // 非局部跳转
#include <cuchar>      // Unicode 字符转换
#include <cwchar>      // 宽字符
#include <cwctype>     // 宽字符分类

static jmp_buf checkpoint;                  // longjmp 的"存档点"

static void risky_jump(int depth) {
    std::println("  深入到深度 {}，触发 longjmp", depth);
    longjmp(checkpoint, depth);             // 跳回 setjmp 处（跨函数！）
}

int main() {
    // <cstddef>：基础类型与 byte（C++ 特有的字节类型）
    std::size_t n = 10;
    std::byte b{0xFF};                      // byte 是 enum class，位运算要显式
    std::println("byte: {:x} 与 0x0F 后={:x} (void*) 大小={}", (unsigned)b,
                 (unsigned)(b & std::byte{0x0F}), sizeof(std::byte));
    (void)n;

    // <cstdint>/<climits>/<cfloat>：与 C 同名宏
    std::println("INT_MAX={} CHAR_BIT={} DBL_DIG={}", INT_MAX, CHAR_BIT, DBL_DIG);

    // <csetjmp>：setjmp/longjmp —— C 时代的"异常"，C++ 里禁与异常/RAII 混用
    int jumped = setjmp(checkpoint);        // 首次返回 0；longjmp 后返回传入值
    if (jumped == 0) {
        risky_jump(3);                      // 不会返回到这里，直接跳回 setjmp
        std::println("这行永远不会执行");
    } else {
        std::println("longjmp 带回值 {}", jumped);
    }

    // <cuchar>：Unicode 转换（char8_t/char16_t/char32_t 是 C++20 独立类型）
    char8_t  u8  = u8'A';                   // UTF-8 码元（u8 字面量只能放 ASCII）
    char16_t u16 = u'好';                   // UTF-16 码元
    char32_t u32 = U'好';                   // UTF-32 码点
    std::println("'好'码点={:x}（U+597D）'A'码点={:x} 宽度 {} {} {}",
                 (unsigned)u32, (unsigned)u8, sizeof u8, sizeof u16, sizeof u32);

    // <cwchar>：宽字符字符串（L"" 前缀）
    const wchar_t *ws = L"宽字符";
    std::println("wide 长度={} 第一个字面值={:#x}", std::wcslen(ws), (unsigned)ws[0]);

    // <cwctype>：宽字符分类
    std::println("iswalpha(L'a')={} towupper(L'x')={}",
                 std::iswalpha(L'a') != 0, (char)std::towupper(L'x'));
    return 0;
}
