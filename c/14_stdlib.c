// 14 - <stdlib.h>：转换、排序、查找、随机数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// qsort 需要的比较函数：返回 <0 / 0 / >0
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);          // 防溢出的写法，不用 x - y
}

int main(void) {
    // 字符串 → 数字：strtol 比 atoi 安全（带错误检测与进制）
    char *end;
    long v = strtol("  123abc", &end, 10);
    printf("strtol=%ld 停在'%s'\n", v, end);
    printf("atoi=%d atof=%g\n", atoi("-42"), atof("3.5e2"));

    // qsort：任意类型的通用排序（函数指针 + void* 实现泛型）
    int a[] = {5, 2, 8, 1, 9, 3};
    qsort(a, 6, sizeof a[0], cmp_int);
    printf("qsort: ");
    for (int i = 0; i < 6; i++) printf("%d ", a[i]);
    putchar('\n');

    // bsearch：二分查找（要求数组已排序）
    int key = 8;
    int *found = bsearch(&key, a, 6, sizeof a[0], cmp_int);
    printf("bsearch(8)=%d\n", found ? *found : -1);

    // 随机数：确定性种子让输出可复现
    srand(42);
    printf("rand: %d %d %d\n", rand() % 100, rand() % 100, rand() % 100);

    // 动态环境/其他
    printf("abs(-9)=%d div: %d 余 %d\n", abs(-9), div(17, 5).quot, div(17, 5).rem);
    return 0;
}
