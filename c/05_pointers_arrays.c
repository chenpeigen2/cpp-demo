// 05 - 指针与数组
#include <stdio.h>

int main(void) {
    int x = 10;
    int *p = &x;                 // p 存 x 的地址
    *p = 20;                     // 解引用写入
    printf("x=%d p=%p *p=%d\n", x, (void *)p, *p);

    // 数组名在表达式中退化为首元素指针
    int a[5] = {10, 20, 30, 40, 50};
    int *q = a;                  // 等价于 &a[0]
    printf("a[2]=%d *(a+2)=%d q[3]=%d\n", a[2], *(a + 2), q[3]);
    q++;                         // 指针算术按元素大小步进
    printf("++q -> *q=%d\n", *q);

    // 数组与指针可互换：a[i] 就是 *(a+i)
    printf("%d %d\n", 2[a], *(q + 1));   // 2[a] 也合法，但别这么写

    // 二维数组：按行连续存放
    int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int (*row)[3] = m;           // 指向"含3个int的数组"
    printf("m[1][2]=%d %d\n", m[1][2], row[1][2]);

    // void* 是"泛型指针"，可存任何对象地址，但不能解引用
    void *v = a;
    printf("*(int*)v=%d sizeof(void*)=%zu\n", *(int *)v, sizeof(void *));

    // 指针的指针
    int **pp = &p;
    printf("**pp=%d\n", **pp);
    return 0;
}
