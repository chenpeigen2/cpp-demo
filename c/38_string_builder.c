// 38 - 实战：StringBuilder —— 倍增缓冲 vs strcat 的 O(n²) 灾难（实测）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

// ---- StringBuilder：缓冲 + 长度，倍增扩容 ----
typedef struct { char *buf; size_t len, cap; } SB;

static void sb_init(SB *s) { s->buf = malloc(16); s->buf[0] = 0; s->len = 0; s->cap = 16; }

static void sb_append(SB *s, const char *str) {
    size_t n = strlen(str);
    if (s->len + n + 1 > s->cap) {                       // 先判断，再扩容
        size_t ncap = s->cap * 2;
        while (s->len + n + 1 > ncap) ncap *= 2;
        s->buf = realloc(s->buf, ncap);
        s->cap = ncap;
    }
    memcpy(s->buf + s->len, str, n + 1);                 // 直接写尾部：O(n) 一次
    s->len += n;
}

#define PARTS 4000

int main(void) {
    // ---- 错误做法：strcat 到长字符串头部 → 每次都从头扫到尾 ----
    size_t big = PARTS * 2 + 1;
    char *naive = malloc(big);
    naive[0] = 0;
    double t0 = now_ns();
    for (int i = 0; i < PARTS; i++)
        strcat(naive, "ab");                             // strcat 先找 '\0'：O(len)！
    double t_naive = now_ns() - t0;

    // ---- 正确做法：builder 记住长度，只写尾部 ----
    SB sb;
    sb_init(&sb);
    t0 = now_ns();
    for (int i = 0; i < PARTS; i++)
        sb_append(&sb, "ab");
    double t_sb = now_ns() - t0;

    printf("拼接 %d 段（每段 2 字节）：\n", PARTS);
    printf("  strcat 逐段     %9.2f ms   复杂度 O(n²)：第 i 段要扫过前 i 段\n", t_naive / 1e6);
    printf("  StringBuilder   %9.2f ms   复杂度 O(n)：直接 memcpy 到尾部\n", t_sb / 1e6);
    printf("  差距 %.0f 倍（n 越大差距越大，按平方增长）\n", t_naive / t_sb);
    printf("  结果一致: %s（%zu 字节）\n", strncmp(naive, sb.buf, big) == 0 ? "相同" : "不同", sb.len);

    // 展示倍增轨迹
    printf("\n倍增扩容轨迹: ");
    SB s2;
    sb_init(&s2);
    size_t last_cap = s2.cap;
    for (int i = 0; i < 20; i++) {
        sb_append(&s2, "xxxx");
        if (s2.cap != last_cap) { printf("%zu→%zu ", last_cap, s2.cap); last_cap = s2.cap; }
    }
    printf("\n结论：任何'循环拼接'都要用 builder 模式（C 的realloc倍增 / C++的string+=\n"
           "     / Python的join / Java的StringBuilder），复杂度差一个数量级\n");
    free(naive);
    free(sb.buf);
    free(s2.buf);
    return 0;
}
