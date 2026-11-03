// 02 - 基本类型与字面量
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    // sizeof 给出类型/表达式的字节数，返回 size_t（用 %zu 打印）
    printf("char=%zu short=%zu int=%zu long=%zu long long=%zu\n",
           sizeof(char), sizeof(short), sizeof(int),
           sizeof(long), sizeof(long long));
    printf("float=%zu double=%zu long double=%zu _Bool=%zu\n",
           sizeof(float), sizeof(double), sizeof(long double), sizeof(bool));

    // 整数字面量：十进制 / 八进制 / 十六进制 / 二进制(C23) / 数字分隔符(C23)
    printf("%d %d %d %d %d\n", 42, 052, 0x2A, 0b101010, 1'000'000);

    // 后缀决定类型：u 无符号、l/ll long、f float、无后缀小数为 double
    char   c = 'A';              // 字符字面量是 int，值为 ASCII 65
    float  f = 3.14f;
    double d = 1e9;              // 科学计数法
    printf("%c=%d %g %g\n", c, c, f, d);

    // 字符串字面量类型是 char[]（只读），以 '\0' 结尾
    char *s = "你好";            // UTF-8 编码，占 6 字节
    printf("%s 占 %zu 字节（不含 '\\0'）\n", s, sizeof("你好") - 1);
    return 0;
}
