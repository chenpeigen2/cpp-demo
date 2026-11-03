// 25 - POSIX 线程（pthread）：C11 <threads.h> 在 macOS SDK 缺失，
// pthread 是 macOS/Linux 上 C 多线程的事实标准，API 模式与 C11 threads 一致
#include <stdio.h>
#include <pthread.h>

typedef struct {            // 传给线程的参数用 struct 打包
    int id;
    int start;
    int count;
    long long result;
} Job;

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static long long g_total = 0;

static void *compute(void *arg) {
    Job *j = arg;
    long long sum = 0;
    for (int i = 0; i < j->count; i++)
        sum += j->start + i;
    j->result = sum;

    // 共享数据要互斥保护
    pthread_mutex_lock(&g_mutex);
    g_total += sum;
    pthread_mutex_unlock(&g_mutex);
    return NULL;                 // 返回值可由 pthread_join 取出
}

int main(void) {
    pthread_t threads[2];
    Job jobs[2] = {{1, 1, 100, 0}, {2, 101, 100, 0}};

    for (int i = 0; i < 2; i++)
        pthread_create(&threads[i], NULL, compute, &jobs[i]);
    for (int i = 0; i < 2; i++)
        pthread_join(threads[i], NULL);          // 等待结束（类似 join）

    printf("job1 sum=%lld job2 sum=%lld\n", jobs[0].result, jobs[1].result);
    printf("g_total=%lld（期望 20100）\n", g_total);

    // 条件变量：线程间等待/通知
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    printf("cond ready=%d\n", pthread_cond_signal(&cond) == 0);

    // 分离线程：结束后自动回收资源，不能 join
    pthread_t dt;
    pthread_create(&dt, NULL, compute, &jobs[0]);
    pthread_detach(dt);
    printf("mutex destroy=%d cond destroy=%d\n",
           pthread_mutex_destroy(&g_mutex), pthread_cond_destroy(&cond));
    return 0;
}
