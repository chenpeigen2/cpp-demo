// 35 - 实战：微基准测量（单调时钟 + ns/op 数据表）
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);      // 单调时钟：不受系统改时影响
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

#define ITERS 50
static char g_src[65536], g_dst[65536];

static double bench_memcpy(void) {           // memcpy（SIMD 加速）vs 逐字节循环
    memset(g_src, 1, sizeof g_src);
    double t0 = now_sec();
    for (int i = 0; i < ITERS; i++) memcpy(g_dst, g_src, sizeof g_src);
    return (now_sec() - t0) / ITERS * 1e9;
}

static double bench_byte_loop(void) {
    double t0 = now_sec();
    for (int i = 0; i < ITERS; i++)
        for (size_t j = 0; j < sizeof g_src; j++) g_dst[j] = g_src[j];
    return (now_sec() - t0) / ITERS * 1e9;
}

static double bench_stack_sum(void) {        // 栈数组 vs 堆数组顺序求和（应无差别）
    static int a[100000];
    for (int i = 0; i < 100000; i++) a[i] = i;
    double t0 = now_sec();
    long long sink = 0;
    for (int k = 0; k < ITERS; k++)
        for (int i = 0; i < 100000; i++) sink += a[i];
    if (sink == 1) printf("");
    return (now_sec() - t0) / ITERS * 1e9;
}

static double bench_heap_sum(void) {
    int *a = malloc(100000 * sizeof *a);
    for (int i = 0; i < 100000; i++) a[i] = i;
    double t0 = now_sec();
    long long sink = 0;
    for (int k = 0; k < ITERS; k++)
        for (int i = 0; i < 100000; i++) sink += a[i];
    if (sink == 1) printf("");
    double r = (now_sec() - t0) / ITERS * 1e9;
    free(a);
    return r;
}

static double bench_strlen(void) {           // 循环内重复 strlen 的代价
    static char s[5000];
    memset(s, 'x', sizeof s - 1);
    double t0 = now_sec();
    long long sink = 0;
    for (int k = 0; k < ITERS; k++)
        for (size_t i = 0; i < strlen(s); i++) sink += s[i];   // 每圈 O(n) strlen
    if (sink == 1) printf("");
    return (now_sec() - t0) / ITERS * 1e9;
}

static double bench_strlen_cached(void) {
    static char s[5000];
    memset(s, 'x', sizeof s - 1);
    double t0 = now_sec();
    long long sink = 0;
    for (int k = 0; k < ITERS; k++) {
        size_t n = strlen(s);                                 // 只算一次
        for (size_t i = 0; i < n; i++) sink += s[i];
    }
    if (sink == 1) printf("");
    return (now_sec() - t0) / ITERS * 1e9;
}

int main(void) {
    // 预热 + 正式测量，输出数据表
    printf("%-28s %12s\n", "基准", "ns/op");
    printf("%-28s %12.0f\n", "memcpy 64KB", bench_memcpy());
    printf("%-28s %12.0f\n", "逐字节循环 64KB", bench_byte_loop());
    printf("%-28s %12.0f\n", "栈数组求和 100K", bench_stack_sum());
    printf("%-28s %12.0f\n", "堆数组求和 100K", bench_heap_sum());
    printf("%-28s %12.0f\n", "循环内 strlen(5000)", bench_strlen());
    printf("%-28s %12.0f\n", "缓存 strlen(5000)", bench_strlen_cached());
    printf("\n解读：memcpy 借助 SIMD 比手写快一个量级；strlen 是 O(n)，\n"
           "循环条件里每圈重算 → 缓存长度可提速数千倍。\n");
    return 0;
}
