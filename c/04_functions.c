// 04 - 函数：原型、递归、inline、函数指针、C23 属性
#include <stdio.h>

// 函数原型（声明）让编译器先知道签名
int square(int x);

// C23 标准属性：提示调用方不要丢弃返回值
[[nodiscard]] int important(void) { return 1; }

// static = 内部链接：仅本文件可见
static int helper(int x) { return x + 1; }

// inline 是给优化器的提示（C 里必须配 static 才有可链接的定义）
static inline int add(int a, int b) { return a + b; }

// 递归
static int fact(int n) { return n <= 1 ? 1 : n * fact(n - 1); }

// 接收函数指针：把"行为"当参数传
static int apply(int (*fp)(int), int v) { return fp(v); }

int square(int x) { return x * x; }

int main(void) {
    printf("square(5)=%d add=%d fact(5)=%d helper=%d\n",
           square(5), add(2, 3), fact(5), helper(9));
    // 想丢弃返回值时必须显式 (void)，否则警告；此处故意触发一次再用 pragma 压掉
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"
    important();
#pragma clang diagnostic pop
    (void)important();

    int (*fp)(int) = square;           // 函数指针：&可省略
    printf("fp(6)=%d apply(fp,7)=%d\n", fp(6), apply(square, 7));
    return 0;
}
