// 22 - <ctype.h> 与 <locale.h>：字符分类与本地化
#include <stdio.h>
#include <ctype.h>
#include <locale.h>

int main(void) {
    // 字符分类：参数是 int（通常传 unsigned char 转换后的值）
    printf("isdigit('7')=%d isalpha('a')=%d isspace(' ')=%d\n",
           !!isdigit('7'), !!isalpha('a'), !!isspace(' '));
    printf("isupper('A')=%d islower('a')=%d isxdigit('f')=%d\n",
           !!isupper('A'), !!islower('a'), !!isxdigit('f'));
    printf("ispunct('!')=%d iscntrl('\\n')=%d isprint('~')=%d\n",
           !!ispunct('!'), !!iscntrl('\n'), !!isprint('~'));

    // 大小写转换
    char s[] = "Hello, World 123";
    for (char *p = s; *p; p++)
        putchar(islower(*p) ? toupper(*p) : (isupper(*p) ? tolower(*p) : *p));
    putchar('\n');

    // locale：C 程序默认是 "C" locale（不认识 UTF-8 等多字节字符）
    // setlocale 让库函数感知环境区域设置；这里显式设回 "C" 展示 API
    char *cur = setlocale(LC_ALL, "C");
    printf("locale=%s\n", cur ? cur : "(null)");

    // localeconv：读当前 locale 的数字/货币格式约定
    struct lconv *lc = localeconv();
    printf("小数点='%s' 千分位='%s'\n", lc->decimal_point, lc->thousands_sep);

    // 常见组合：手写词法分析时用 ctype 判类别
    const char *src = "int x = 42;";
    printf("首字符 %s\n", isalpha(src[0]) ? "是标识符起始" : "不是");
    return 0;
}
