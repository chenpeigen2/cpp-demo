// 09 - 预处理器：宏、条件编译、预定义宏
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))   // 函数式宏：参数必须加括号
#define STR(x)   #x                          // # 把参数变成字符串
#define XSTR(x)  STR(x)                      // 两层才能展开宏后再字符串化
#define CAT(a, b) a##b                       // ## 拼接记号

// 可变参数宏：__VA_ARGS__ 展开成全部实参（实际用法见 16_stdarg.c）
#define LOGF(fmt, ...) printf("[log] " fmt "\n", __VA_ARGS__)

#if defined(DEBUG)                             // 条件编译：-DDEBUG 开启
#define LOG(msg) puts("DEBUG: " msg)
#else
#define LOG(msg) ((void)0)
#endif

int main(void) {
    printf("MAX(3,9)=%d STR(hi)=%s\n", MAX(3, 9), STR(hi));

    int value = 7;                             // ## 把两个记号拼成一个名字
    printf("CAT(va,lue)=%d XSTR(MAX)=%s\n", CAT(va, lue), XSTR(MAX));

    // 预定义宏
    printf("file=%s line=%d func=%s date-style=%s\n",
           __FILE__, __LINE__, __func__, __STDC_VERSION__ >= 202311L ? "C23" : "older");

    LOG("compiled without -DDEBUG");

    // _Pragma：宏内可用的编译指示（等价 #pragma）
    _Pragma("clang diagnostic push")
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"")
    int unused_var = 0;
    _Pragma("clang diagnostic pop")
    (void)unused_var;
    return 0;
}
