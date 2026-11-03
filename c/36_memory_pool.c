// 36 - 实战：内存池 —— bump 分配器 + 固定块 free-list（分配不再走 malloc）
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

static double now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

// ---- Arena（bump 分配器）：指针往前推，绝不单独回收 ----
typedef struct { char *buf; size_t cap, used; size_t allocs; } Arena;

static void arena_init(Arena *a, size_t cap) {
    a->buf = malloc(cap);
    a->cap = cap; a->used = 0; a->allocs = 0;
}
static void *arena_alloc(Arena *a, size_t n) {
    uintptr_t base = (uintptr_t)a->buf + a->used;
    uintptr_t aligned = (base + 15) & ~(uintptr_t)15;   // 对齐到 16
    size_t off = aligned - (uintptr_t)a->buf;
    if (off + n > a->cap) return NULL;                  // 不够就失败（可扩容/换页）
    a->used = off + n;
    a->allocs++;
    return a->buf + off;
}
static void arena_reset(Arena *a) { a->used = 0; }      // 一次性全部回收（帧分配模式）

// ---- 固定块池：free-list 穿在块内存里（0 额外空间存链表）----
typedef struct Block { struct Block *next; } Block;
typedef struct { Block *base; Block *free_head; size_t total, live; } Pool;

static void pool_init(Pool *p, size_t n) {
    p->base = (Block *)malloc(n * sizeof(Block));
    p->free_head = p->base;
    for (size_t i = 0; i < n - 1; i++) p->base[i].next = &p->base[i + 1];  // 串成链
    p->base[n - 1].next = NULL;
    p->total = n; p->live = 0;
}
static void *pool_alloc(Pool *p) {
    if (!p->free_head) return NULL;
    Block *b = p->free_head;             // O(1)：摘链表头
    p->free_head = b->next;
    p->live++;
    return b;
}
static void pool_free(Pool *p, void *ptr) {
    Block *b = ptr;                      // O(1)：头插回去
    b->next = p->free_head;
    p->free_head = b;
    p->live--;
}

#define ALLOCS 200000
int main(void) {
    Arena a;
    arena_init(&a, 1 << 20);
    // 从 arena 连续分配 1000 个 24 字节结构，观察地址紧贴
    void *first = arena_alloc(&a, 24);
    void *second = arena_alloc(&a, 24);
    printf("arena: first=%p second=%p 相距=%zu 字节（就是 24，零簿记开销）\n",
           first, second, (size_t)((char *)second - (char *)first));

    // 基准：malloc vs arena vs pool
    double t0 = now_ns();
    for (int i = 0; i < ALLOCS; i++) { void *p = malloc(64); free(p); }
    double t_malloc = (now_ns() - t0) / ALLOCS;

    t0 = now_ns();
    for (int i = 0; i < ALLOCS; i++) (void)arena_alloc(&a, 64);
    double t_arena = (now_ns() - t0) / ALLOCS;

    Pool p;
    pool_init(&p, 1024);
    void *handles[256] = {0};
    t0 = now_ns();
    for (int i = 0; i < ALLOCS; i++) {
        int slot = i % 256;
        if (handles[slot]) pool_free(&p, handles[slot]);   // 先还旧块，再取新块：零泄漏
        handles[slot] = pool_alloc(&p);
    }
    double t_pool = (now_ns() - t0) / ALLOCS;

    printf("\n单次分配耗时（%d 次，64 字节）：\n", ALLOCS);
    printf("  malloc/free  %8.1f ns（带锁、找桶、合并碎片）\n", t_malloc);
    printf("  arena bump   %8.1f ns（一条加法，快 %.0f 倍）\n", t_arena, t_malloc / t_arena);
    printf("  free-list    %8.1f ns（两次指针移动，快 %.0f 倍）\n", t_pool, t_malloc / t_pool);
    printf("\n使用纪律：arena 适合'同生共死'的批量数据（每帧/每请求/每查询）\n"
           "         reset 即回收，杜绝内存泄漏；绝不对 arena 内存单独 free\n");
    arena_reset(&a);
    free(a.buf);
    free(p.base);          // 注意：释放原始基址，free_head 只是链表游标
    return 0;
}
