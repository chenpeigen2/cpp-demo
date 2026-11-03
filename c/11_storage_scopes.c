// 11 - 存储类别、作用域与生命周期
#include <stdio.h>

int g = 1;                    // 文件作用域（全局），整个程序生命周期
static int sg = 2;            // internal 链接：仅本文件可见
extern int g;                 // 声明"g 在别处定义"，这里重新引用

void counter(void) {
    static int n = 0;         // static 局部变量：只初始化一次，跨调用保值
    n++;
    printf("第%d次调用\n", n);
}

int main(void) {
    counter(); counter();

    printf("g=%d sg=%d\n", g, sg);

    // 块作用域：内层变量遮蔽外层，出块即销毁
    int x = 10;
    {
        int x = 20;           // 遮蔽外层 x
        printf("inner x=%d\n", x);
    }
    printf("outer x=%d\n", x);

    // VLA（变长数组）：长度运行期决定，分配在栈上
    int n = 3;
    int vla[n];
    for (int i = 0; i < n; i++) vla[i] = i * i;
    printf("VLA: %d %d %d\n", vla[0], vla[1], vla[2]);

    // const：只读修饰；指向 const 的指针 vs const 指针
    const int ci = 5;
    int v = 6;
    const int *p1 = &v;       // 指向 const 的指针（*p1 不可写）
    int *const p2 = &v;       // const 指针（p2 本身不可变）
    printf("%d %d %d\n", ci, *p1, *p2);
    return 0;
}
