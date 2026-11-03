// 16 - <string.h>：字符串与内存函数
#include <stdio.h>
#include <string.h>

int main(void) {
    // 比较：strncmp 限制长度；返回值是字典序差值符号
    printf("strcmp(abc,abd)=%d strncmp(abc,abd,2)=%d\n",
           strcmp("abc", "abd"), strncmp("abc", "abd", 2));

    // 查找：strchr 找字符，strstr 找子串，strcspn 找"任意字符首次出现"
    const char *s = "hello, world";
    printf("strchr(o)=%s\n", strchr(s, 'o'));
    printf("strrchr(o)=%s\n", strrchr(s, 'o'));      // 从后往前
    printf("strstr(world)=%s\n", strstr(s, "world"));
    printf("strcspn(, )=%zu（逗号前的长度）\n", strcspn(s, ","));

    // 切分：strtok 会修改原串，用 '\0' 替换分隔符（有内部状态，非线程安全）
    char csv[] = "a,b,c";
    for (char *t = strtok(csv, ","); t; t = strtok(NULL, ","))
        printf("token=%s ", t);
    putchar('\n');

    // 内存函数：对任意字节块操作，遇 '\0' 不停
    char dst[8];
    memcpy(dst, "abcdefg", 8);          // 含 '\0' 共 8 字节
    printf("memcpy=%s\n", dst);

    // memmove 允许源/目标重叠（memcpy 重叠是 UB！）
    char ov[] = "1234567";
    memmove(ov + 2, ov, 5);             // 向后搬，重叠
    printf("memmove=%s\n", ov);

    // memset 按字节填充；memcmp 按字节比较
    char z[4];
    memset(z, 'x', 3); z[3] = 0;
    printf("memset=%s memcmp=%d\n", z, memcmp("abc", "abd", 2));
    return 0;
}
