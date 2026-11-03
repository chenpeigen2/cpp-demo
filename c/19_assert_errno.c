// 19 - <assert.h> 与 <errno.h>：断言与错误码
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void) {
    // assert：运行期检查，失败打印表达式+位置并中止
    assert(1 + 1 == 2);

    // errno：库函数失败时设置的错误码；errno.h 定义 EDOM/ERANGE 等
    errno = 0;
    double r = sqrt(-1.0);                 // 定义域错误
    if (errno == EDOM)
        printf("sqrt(-1) 触发 EDOM，返回 %g\n", r);

    errno = 0;
    long big = strtol("99999999999999999999", NULL, 10);  // 超出 long 范围
    if (errno == ERANGE)
        printf("strtol 溢出触发 ERANGE，返回 %ld\n", big);

    // strerror 把错误码翻译成字符串；perror 直接打印
    errno = ENOENT;                        // No such file or directory
    printf("strerror(ENOENT)=%s\n", strerror(errno));

    // 静态断言（C11 起）：编译期检查，零运行时成本
    static_assert(sizeof(int) >= 4, "需要至少 4 字节的 int");
    static_assert(sizeof(void *) == 8, "64 位指针");
    puts("static_assert 通过");
    return 0;
}
