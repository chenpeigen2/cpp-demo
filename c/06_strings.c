// 06 - C 字符串：本质是 '\0' 结尾的 char 数组
#include <stdio.h>
#include <string.h>

int main(void) {
    char buf[16] = "abc";        // 剩余元素全填 '\0'
    printf("strlen=%zu sizeof=%zu\n", strlen(buf), sizeof(buf)); // 3 vs 16

    // 字符串字面量存放在只读区，试图修改是未定义行为
    char *lit = "immutable";
    printf("%s length=%zu\n", lit, strlen(lit));

    // 可修改的字符串要在数组/堆里
    char s[32];
    strcpy(s, "hello");          // 拷贝（含 '\0'）
    strcat(s, ", C");            // 拼接
    printf("%s\n", s);

    // 手动遍历：遇到 '\0' 停
    for (char *p = s; *p; p++) putchar(*p);
    putchar('\n');

    // 转义字符
    printf("tab\t|newline done|quote\"%c|percent%%|backslash\\\n", '"');
    return 0;
}
