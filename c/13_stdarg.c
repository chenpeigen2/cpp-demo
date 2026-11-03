// 13 - <stdarg.h>：可变参数函数
#include <stdio.h>
#include <stdarg.h>

// 求任意个整数的和；C 的变参没有类型信息，必须靠调用约定（此处约定 -1 结尾）
static int sum(int first, ...) {
    va_list ap;
    va_start(ap, first);          // ap 指向 first 之后的第一个参数
    int total = first;
    int v;
    while ((v = va_arg(ap, int)) != -1)   // 逐个取出（类型必须与实参匹配）
        total += v;
    va_end(ap);                   // 清理
    return total;
}

// printf 风格：fmt 也是运行期信息
static double average(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double total = 0;
    for (int i = 0; i < count; i++)
        total += va_arg(ap, double);      // 浮点提升后都是 double
    va_end(ap);
    return total / count;
}

// vprintf：把 va_list 转发给 printf 家族（自己写 log 函数的常用套路）
static void logf_(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

int main(void) {
    printf("sum(1,2,3) = %d\n", sum(1, 2, 3, -1));
    printf("avg(2.0,4.0,6.0) = %g\n", average(3, 2.0, 4.0, 6.0));
    logf_("x=%d y=%g s=%s\n", 7, 0.5, "done");
    return 0;
}
