// 03 - 控制流
#include <stdio.h>

int main(void) {
    int n = 7;

    if (n % 2 == 0)      puts("偶数");
    else if (n % 3 == 0) puts("能被3整除");   // n=7 走这里
    else                 puts("其他");

    // switch：case 必须是编译期整数常量；break 防止"贯穿"
    switch (n) {
        case 7:  puts("seven"); break;
        case 8:  puts("eight"); break;
        default: puts("other");
    }

    // 三种循环
    for (int i = 0; i < 3; i++) printf("for %d ", i);
    int j = 0;
    while (j < 2) { printf("while %d ", j++); }
    int k = 100;
    do { printf("do %d", k); } while (0);   // 至少执行一次
    putchar('\n');

    // break 跳出循环，continue 跳过本轮
    for (int i = 0; i < 5; i++) {
        if (i == 1) continue;
        if (i == 4) break;
        printf("%d ", i);                    // 0 2 3
    }
    putchar('\n');

    // goto 只能跳到同函数内的标号；合理场景是从深层嵌套清理退出
    for (int i = 0; i < 3; i++)
        for (int p = 0; p < 3; p++)
            if (i * p == 2) goto done;
done:
    puts("goto 跳出了双重循环");
    return 0;
}
