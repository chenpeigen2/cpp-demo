// 24 - <stdatomic.h>：C11 原子操作与无锁编程
#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

static atomic_int counter = 0;              // 原子计数器
static atomic_flag lock = ATOMIC_FLAG_INIT; // 最简单的自旋锁原语

static void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 10000; i++)
        atomic_fetch_add(&counter, 1);      // 原子 +1，默认顺序一致性
    return NULL;
}

static void *lock_worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 100; i++) {
        // 自旋锁：test_and_set 原子地"检查并置位"，返回旧值
        while (atomic_flag_test_and_set(&lock))
            ;                                // 别人持锁，转圈等
        // ---- 临界区 ----
        // （这里放需要互斥的代码）
        atomic_flag_clear(&lock);            // 释放
    }
    return NULL;
}

int main(void) {
    // 两个线程并发自增 2 万次，原子操作保证结果正确
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("atomic counter=%d（期望 20000）\n", atomic_load(&counter));

    // 比较交换（CAS）：无锁数据结构的核心操作
    int expected = 0;
    atomic_store(&counter, 5);
    _Bool ok = atomic_compare_exchange_strong(&counter, &expected, 99);
    printf("CAS(5→99): ok=%d old=%d now=%d\n", ok, expected, atomic_load(&counter));
    expected = 0;                            // 期望值改错，CAS 会失败并回读实际值
    ok = atomic_compare_exchange_strong(&counter, &expected, 100);
    printf("CAS(0→100): ok=%d old=%d now=%d\n", ok, expected, atomic_load(&counter));

    // 顺序控制：acquire/release 配对建立同步关系
    atomic_store_explicit(&counter, 7, memory_order_release);
    int v = atomic_load_explicit(&counter, memory_order_acquire);
    printf("acquire load=%d\n", v);

    pthread_t l1, l2;
    atomic_flag_clear(&lock);
    pthread_create(&l1, NULL, lock_worker, NULL);
    pthread_create(&l2, NULL, lock_worker, NULL);
    pthread_join(l1, NULL);
    pthread_join(l2, NULL);
    puts("spinlock ok");
    return 0;
}
