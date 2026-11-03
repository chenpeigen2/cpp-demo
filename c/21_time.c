// 21 - <time.h>：时间与日期
#include <stdio.h>
#include <time.h>
#include <math.h>

static void spin(double seconds) {          // 忙等指定秒数（演示用）
    clock_t start = clock();
    while ((double)(clock() - start) / CLOCKS_PER_SEC < seconds)
        ;                                    // 空转
}

int main(void) {
    // time_t：日历时间（秒）；clock_t：进程 CPU 时间（毫秒精度）
    time_t now = time(NULL);
    printf("epoch 秒数=%lld\n", (long long)now);

    // localtime + strftime：格式化成可读字符串
    struct tm *lt = localtime(&now);
    char buf[64];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S %A", lt);
    printf("本地时间=%s\n", buf);
    printf("tm 结构: 年=%d(从1900) 月=%d(0起) 日=%d\n",
           lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);

    // mktime：反向把 tm 转回 time_t（可做日期运算）
    struct tm t = *lt;
    t.tm_mday += 7;                          // 一周后
    time_t later = mktime(&t);
    printf("一周后=%.24s\n", ctime(&later)); // ctime 是快捷格式化

    // CPU 时间与真实时间：CLOCKS_PER_SEC 换算
    spin(0.05);
    printf("CPU 时间=%.3f 秒\n", (double)clock() / CLOCKS_PER_SEC);

    // 真实时间用 clock_gettime（POSIX 扩展）：CLOCK_MONOTONIC 不受系统改时影响
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("monotonic=%ld.%09ld 秒\n", (long)ts.tv_sec, (long)ts.tv_nsec);

    // difftime 计算两个 time_t 的差（秒）
    printf("diff=%.0f 秒\n", difftime(later, now));
    return 0;
}
