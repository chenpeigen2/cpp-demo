// 12 - C11 泛型选择 _Generic：编译期按表达式类型分派
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// _Generic 不做求值，只看类型，类似 C++ 重载但发生在预处理之后
#define TYPE_NAME(x) _Generic((x),            \
    int:     "int",                           \
    long:    "long",                          \
    double:  "double",                        \
    char *:  "char*",                         \
    float:   "float",                         \
    default: "unknown")

// 实用例子：按类型选绝对值函数
#define ABS(x) _Generic((x),                  \
    int:        abs,                          \
    long:       labs,                         \
    float:      fabsf,                        \
    double:     fabs)(x)

int main(void) {
    printf("42      -> %s\n", TYPE_NAME(42));
    printf("42L     -> %s\n", TYPE_NAME(42L));
    printf("3.14    -> %s\n", TYPE_NAME(3.14));
    printf("\"hi\"    -> %s\n", TYPE_NAME("hi"));
    printf("1.5f    -> %s\n", TYPE_NAME(1.5f));

    printf("ABS(-7)=%d ABS(-2.5f)=%g ABS(-3.9)=%g\n",
           ABS(-7), ABS(-2.5f), ABS(-3.9));
    return 0;
}
