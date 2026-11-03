// 08 - 堆内存：malloc / calloc / realloc / free
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // malloc：分配未初始化内存；free 后指针应置空
    int *a = malloc(4 * sizeof *a);          // 惯用法：sizeof *p 避免写错类型
    if (!a) return 1;                        // 分配失败返回 NULL
    for (int i = 0; i < 4; i++) a[i] = i * i;
    printf("malloc: %d %d %d %d\n", a[0], a[1], a[2], a[3]);

    // realloc：扩大/缩小已分配块（可能搬移数据，返回新指针）
    a = realloc(a, 8 * sizeof *a);
    a[4] = 16;
    printf("realloc: size now %d elems, a[4]=%d\n", 8, a[4]);

    // calloc：清零 + 检查乘法溢出
    int *b = calloc(4, sizeof *b);
    printf("calloc zeroed: %d\n", b[3]);

    free(a);                                 // 忘记 free = 内存泄漏
    free(b);
    a = NULL;                                // 防"悬空指针"误用
    // free(a) 再次 free 同一指针 = 双重释放，未定义行为；对 NULL free 是安全的
    free(a);
    puts("memory ok");

    // 动态二维数组：一块连续内存 + 手动偏移
    int (*m)[3] = malloc(2 * sizeof *m);     // 2 行 x 3 列
    m[1][2] = 99;
    printf("m[1][2]=%d\n", m[1][2]);
    free(m);
    return 0;
}
