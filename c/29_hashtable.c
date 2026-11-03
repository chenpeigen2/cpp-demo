// 29 - 实战：手写哈希表（开放寻址 + 线性探测 + FNV-1a 哈希）
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CAP 16u                       // 容量必须是 2 的幂，& (CAP-1) 即取模

typedef struct {
    const char *key;
    int         val;
    _Bool       used;
} Slot;

static Slot table[CAP];
static size_t collisions = 0;
static size_t probes_max = 0;

// FNV-1a：简单又分布良好的字符串哈希
static uint64_t fnv1a(const char *s) {
    uint64_t h = 1469598103934665603ULL;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 1099511628211ULL;
    }
    return h;
}

static _Bool ht_insert(const char *k, int v) {
    uint64_t h = fnv1a(k);
    size_t i = h & (CAP - 1);
    size_t probes = 1;
    while (table[i].used) {
        if (strcmp(table[i].key, k) == 0) {       // 已存在 → 更新
            table[i].val = v;
            return 1;
        }
        i = (i + 1) & (CAP - 1);                  // 线性探测：下一个槽
        collisions++;
        probes++;
    }
    table[i] = (Slot){k, v, 1};
    if (probes > probes_max) probes_max = probes;
    return 1;
}

static int *ht_find(const char *k) {
    size_t i = fnv1a(k) & (CAP - 1);
    while (table[i].used) {
        if (strcmp(table[i].key, k) == 0) return &table[i].val;
        i = (i + 1) & (CAP - 1);
    }
    return NULL;
}

int main(void) {
    const char *keys[] = {"apple", "banana", "cherry", "date", "elder",
                          "fig", "grape", "honey", "kiwi", "lemon"};
    // 展示哈希值与初始槽位（观察分布）
    for (int i = 0; i < 5; i++)
        printf("%-7s hash=%016llx slot=%zu\n",
               keys[i], (unsigned long long)fnv1a(keys[i]), (size_t)(fnv1a(keys[i]) & (CAP - 1)));

    for (int i = 0; i < 10; i++) ht_insert(keys[i], (i + 1) * 100);
    printf("插入 10 个，碰撞 %zu 次，最长探测 %zu\n", collisions, probes_max);
    printf("负载因子 = 10/%u = %.3f（超过 ~0.7 应 rehash 扩容）\n", CAP, 10.0 / CAP);

    // 查找命中与未命中
    for (int i = 0; i < 10; i++) {
        int *v = ht_find(keys[i]);
        printf("%s→%d ", keys[i], v ? *v : -1);
    }
    putchar('\n');
    printf("find(\"mango\")=%s（NULL=未命中）\n", ht_find("mango") ? "有值" : "NULL");

    // 更新已存在的键
    ht_insert("apple", 999);
    printf("更新后 apple→%d\n", *ht_find("apple"));

    // 没讲到的部分：删除需要"墓碑"标记（直接清 used 会截断探测链）；
    // 负载过高时 rehash：新表容量翻倍、全部键重新哈希。见 cpp/87 的链地址版本。
    return 0;
}
