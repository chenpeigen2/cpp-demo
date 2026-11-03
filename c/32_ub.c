// 32 - 实战：未定义行为 (UB) 展览 —— 知道雷区在哪，才能不踩
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

typedef struct { const char *name; const char *why; const char *fix; } UBItem;

// 每一项都给出"为什么是 UB"和"正确做法"；真正的 UB 代码全部注释，绝不执行
static const UBItem gallery[] = {
    {"有符号溢出", "INT_MAX+1 在补码回绕只是'常见实现'，标准明说未定义（编译器可据此优化掉你的检查）",
     "用无符号回绕、__builtin_add_overflow 或先检查边界"},
    {"数组越界读/写", "越界访问堆栈上的任意内存：安全漏洞和诡异 bug 之源",
     "所有下标进断言；用带长度的 API（fgets/snprintf/memcpy）"},
    {"解引用空/悬空指针", "free 后指针仍指向原地址（悬空），读它是 UB；双重 free 破坏堆",
     "free 后立刻置 NULL；所有权写进注释或用句柄结构"},
    {"读未初始化变量", "int x; printf(x) 读到的是不确定值，还可能是陷阱表示",
     "声明即初始化（int x = 0;）"},
    {"移位 >= 位宽", "1u << 32 在 32 位 int 上是 UB（硬件指令行为各异）",
     "先检查 shift < 位宽，或用固定宽度类型"},
    {"指针类型转换后解引用（违反严格别名）", "把 float* 强转 int* 再读：优化器假设不同类型不共享内存，可能直接删掉你的代码",
     "memcpy 拷贝字节（编译器会优化成寄存器移动），或用 union(C23 前 UB 边界)"},
    {"修改 const 对象", "const int c=1; *(int*)&c=2;——const 数据可能放在只读段，写它直接崩溃",
     "const 就别改；需要改就别标 const"},
    {"序列点之间多次修改同一变量", "i=i++ + ++i：修改之间无顺序，结果随编译器版本变化",
     "拆成多行，每行一个副作用"},
    {"除零/求余为零", "整数除以 0 是 UB（浮点除 0 得 inf，不是 UB）",
     "先判 0"},
    {"字符串字面量写入", "char *s=\"hi\"; s[0]='H'; 字面量在只读段",
     "char s[]=\"hi\"（拷贝进栈数组）"},
};

int main(void) {
    printf("%-32s %-58s 正确做法\n", "UB 名称", "为什么");
    for (unsigned long i = 0; i < sizeof gallery / sizeof gallery[0]; i++)
        printf("%-32s %-58s %s\n", gallery[i].name, gallery[i].why, gallery[i].fix);

    // ---- 两个"边界上的良定义行为"演示 ----
    // 无符号回绕是【明确定义】的（模 2^N），这是它与有符号溢出的本质区别
    unsigned u = 0u - 1u;
    printf("\n无符号回绕(良定义): 0u-1u = %u\n", u);

    // 有符号溢出的安全替代：__builtin_add_overflow（clang/gcc 都支持）
    int r;
    _Bool of = __builtin_add_overflow(INT_MAX, 1, &r);
    printf("INT_MAX+1: 溢出=%d 结果保持 %d\n", of, r);

    // 严格别名的正确示范：memcpy 位转换（等价于错误的 (int&)f 但完全合法）
    float f = -2.75f;
    int32_t bits;
    memcpy(&bits, &f, sizeof bits);
    printf("float -2.75 的位模式(经 memcpy) = 0x%X\n", (unsigned)bits);
    return 0;
}
