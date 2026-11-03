// 01 - 程序基本结构：#include、main、printf、注释
#include <stdio.h>

// main 是程序入口；int 返回值给操作系统（0 = 正常）
// argc/argv 接收命令行参数：argv[0] 是程序名
int main(int argc, char *argv[]) {
    printf("hello, C23!\n");          // \n 换行
    printf("argc=%d argv[0]=%s\n", argc, argv[0]);

    /* 块注释
       可以多行 */
    int x = 42;                       // 行注释
    printf("x=%d\n", x);
    return 0;                         // 不写 return 时 main 默认返回 0
}
