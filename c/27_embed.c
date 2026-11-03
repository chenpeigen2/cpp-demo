// 27 - #embed (C23)：把二进制/文本文件在编译期嵌入为字节序列
#include <stdio.h>

int main(void) {
    // #embed 展开成逗号分隔的字节列表，直接当初始化器用
    static const char blob[] = {
        #embed "embed_data.txt"
        , 0                          // 手动补 '\0'，#embed 本身不含终止符
    };
    printf("嵌入内容: %s（%zu 字节）\n", blob, sizeof blob - 1);

    // limit(n)：只取前 n 个字节
    static const char three[3] = {
        #embed "embed_data.txt" limit(3)
    };
    printf("limit(3): %.3s\n", three);

    // __has_embed：预处理期探测（只能在 #if 等指令里用）
#if __has_embed("embed_data.txt")
    puts("__has_embed 探测到文件");
#else
    puts("文件不存在");
#endif
    // 典型用途：嵌入图标/证书/shader 源码，替代"xxd 生成数组"的老做法
    return 0;
}
