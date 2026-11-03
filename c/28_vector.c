// 28 - 实战：手写泛型动态数组（void* + 元素大小，vector 的 C 原理）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void  *data;      // 元素连续存放
    size_t len;       // 当前元素个数
    size_t cap;       // 已分配容量
    size_t elem;      // 单个元素字节数（"泛型"的关键）
} Vec;

static void vec_init(Vec *v, size_t elem) {
    v->data = NULL; v->len = 0; v->cap = 0; v->elem = elem;
}

static int vec_push(Vec *v, const void *val) {
    if (v->len == v->cap) {                       // 满了 → 扩容（倍增策略）
        size_t ncap = v->cap ? v->cap * 2 : 4;
        void *nd = realloc(v->data, ncap * v->elem);
        if (!nd) return -1;
        v->data = nd;
        v->cap = ncap;
        printf("  扩容: cap=%zu（第 %zu 次分配）\n", ncap, v->cap == 4 ? 1 : (size_t)0 + 1);
    }
    memcpy((char *)v->data + v->len * v->elem, val, v->elem);
    v->len++;
    return 0;
}

static void *vec_get(Vec *v, size_t i) {          // 返回第 i 个元素的地址
    return (char *)v->data + i * v->elem;
}

static void vec_pop(Vec *v) { if (v->len) v->len--; }   // 只缩长度，不缩容量

static void vec_free(Vec *v) { free(v->data); v->data = NULL; v->len = v->cap = 0; }

typedef struct { int x, y; } Point;

int main(void) {
    // ---- 存 int ----
    Vec v;
    vec_init(&v, sizeof(int));
    printf("push 0..11，观察倍增序列：\n");
    for (int i = 0; i < 12; i++) {
        size_t before = v.cap;
        vec_push(&v, &i);
        if (v.cap != before) printf("  len=%zu cap %zu→%zu\n", v.len, before, v.cap);
    }
    int sum = 0;
    for (size_t i = 0; i < v.len; i++) sum += *(int *)vec_get(&v, i);
    printf("len=%zu sum=%d（期望 66）\n", v.len, sum);
    vec_pop(&v);
    printf("pop 后 len=%zu 最后元素=%d\n", v.len, *(int *)vec_get(&v, v.len - 1));
    vec_free(&v);

    // ---- 同一套代码存结构体（这就是泛型）----
    Vec pv;
    vec_init(&pv, sizeof(Point));
    Point pts[3] = {{1, 2}, {3, 4}, {5, 6}};
    for (int i = 0; i < 3; i++) vec_push(&pv, &pts[i]);
    Point *mid = vec_get(&pv, 1);
    printf("Point: (%d,%d)\n", mid->x, mid->y);
    vec_free(&pv);

    // 要点：倍增使 push 均摊 O(1)；realloc 可能搬移内存，旧指针全部失效
    return 0;
}
