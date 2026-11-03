// 10 - 运算符大全
#include <stdio.h>

int main(void) {
    // 算术：/ 对整数截断，% 只用于整数
    printf("7/2=%d 7%%2=%d -7/2=%d 7.0/2=%g\n", 7 / 2, 7 % 2, -7 / 2, 7.0 / 2);

    // 位运算：& 与 | 或 ^ 异或 ~ 取反 << 左移 >> 右移
    unsigned x = 0b1100;
    printf("x&6=%u x|3=%u x^5=%u ~x=%u x<<2=%u x>>2=%u\n",
           x & 6u, x | 3u, x ^ 5u, ~x, x << 2, x >> 2);

    // 赋值复合形式
    int a = 5;
    a += 2; a *= 3; a >>= 1;
    printf("a=%d\n", a);                       // (5+2)*3/2 = 10

    // 自增自减：前置返回新值，后置返回旧值
    // （同一表达式里多次修改同一变量是未序列化 UB，必须拆开写）
    int i = 0;
    int old = i++;                     // old=0, i=1
    int now = ++i;                     // i=2, now=2
    printf("i++ 返回 %d，++i 返回 %d，最终 i=%d\n", old, now, i);

    // 三元条件：可作右值也可作左值（C 标准允许？不行——是 C++；C 中不能当左值）
    int m = 3, n = 8;
    int max = m > n ? m : n;
    printf("max=%d\n", max);

    // 逻辑短路：&& 左边为假则不评估右边；|| 反之
    int called = 0;
    int r = (0 && called++), s = (1 || called++);
    printf("short-circuit: called=%d r=%d s=%d\n", called, r, s);

    // sizeof 是编译期运算符；逗号运算符取最后一个表达式的值
    printf("sizeof(int)=%zu comma=%d\n", sizeof(int), (m = 1, m + 2));

    // 关系运算返回 0/1
    printf("3<5=%d 3==5=%d\n", 3 < 5, 3 == 5);
    return 0;
}
