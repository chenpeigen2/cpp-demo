// 20 - <signal.h>：信号处理
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// 信号处理器：异步打断程序执行时被调用，必须保证"异步信号安全"
// （实践中只做置标志、写 volatile 变量这类最小动作）
static volatile sig_atomic_t got_signal = 0;

static void handler(int sig) {
    got_signal = sig;              // sig 是 SIGUSR1 等编号
}

int main(void) {
    // 注册处理器：SIGUSR1 是留给应用自定义的信号
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    // raise 给自己发信号（模拟外部 kill 命令）
    raise(SIGUSR1);
    printf("收到信号 %d（SIGUSR1=%d）\n", got_signal, SIGUSR1);

    // 忽略某个信号；SIG_DFL 恢复默认行为
    signal(SIGPIPE, SIG_IGN);
    puts("SIGPIPE 已忽略");

    // SIGFPE：算术错误。真除零是 UB，这里仅展示 abort 触发 SIGABRT 的用法
    // （abort 会立即终止程序，core dump 视系统配置）
    printf("SIGABRT 编号=%d\n", SIGABRT);
    return 0;
}
