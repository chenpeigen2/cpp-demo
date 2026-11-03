// 31 - 实战：IEEE-754 浮点内部（拆位看懂 0.1+0.2 != 0.3）
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>

static void show(const char *tag, float f) {
    uint32_t u;
    memcpy(&u, &f, sizeof u);                 // 正确的位查看方式（见 32_ub 的别名规则）
    unsigned sign = u >> 31;
    unsigned exp  = (u >> 23) & 0xFF;
    uint32_t man  = u & 0x7FFFFF;
    printf("%-14s = %-14g 位模式 0x%08X | 符号=%u 指数=%3u(实际%4d) 尾数=0x%06X\n",
           tag, f, u, sign, exp, (int)exp - 127, man);
}

int main(void) {
    // float = 1 位符号 + 8 位指数(偏移127) + 23 位尾数
    show("1.0f", 1.0f);
    show("-2.5f", -2.5f);
    show("0.1f", 0.1f);                       // 注意尾数不是整 0 —— 0.1 无法精确表示
    show("INFINITY", INFINITY);               // 指数全 1，尾数 0
    show("NAN", NAN);                         // 指数全 1，尾数非 0
    show("0.0f", 0.0f);                       // 全 0（符号位区分 ±0）

    // 经典陷阱：0.1+0.2 的二进制误差
    printf("\n0.1+0.2 == 0.3 ? %d（%.17g != %.17g）\n",
           0.1 + 0.2 == 0.3, 0.1 + 0.2, 0.3);

    // float 有效位只有 24 位（约 7 位十进制）：大数吃掉小数
    float big = 1e9f;
    printf("1e9f + 1.0f == 1e9f ? %d（尾数 24 位放不下 1e9+1）\n", big + 1.0f == big);

    // 累积误差：float vs double 累加 0.1 一万次
    float fs = 0;
    double ds = 0;
    for (int i = 0; i < 10000; i++) {
        fs += 0.1f;
        ds += 0.1;
    }
    printf("float 累加 1 万次 = %.2f（期望 1000）  double = %.6f\n", fs, ds);

    // 机器 epsilon：1.0 之后下一个可表示数
    float next = nextafterf(1.0f, 2.0f);
    printf("1.0 的下一个 float = %.9g（差值 %.3g） FLT_EPSILON=%.3g\n",
           next, next - 1.0f, FLT_EPSILON);

    // 正确的浮点比较：用相对误差，不用 ==
    double a = 0.1 + 0.2, b = 0.3;
    printf("fabs(a-b) < 1e-9 ? %d\n", fabs(a - b) < 1e-9);
    printf("十六进制浮点字面量: %a = %g\n", 0.5, 0.5);
    return 0;
}
