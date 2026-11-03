// 15 - <stdio.h>：文件 I/O
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *path = "/tmp/cpp-demo-stdio.txt";

    // 写文件："w" 覆盖创建；文本模式用 fprintf
    FILE *out = fopen(path, "w");
    if (!out) { perror("fopen"); return 1; }   // perror 把 errno 翻译成人类语言
    fprintf(out, "line1 %d\n", 42);
    fputs("line2 hello\n", out);
    fclose(out);                                // 用完必须关

    // 读文件：fgets 按行读（保留换行，安全截断）
    FILE *in = fopen(path, "r");
    char buf[64];
    while (fgets(buf, sizeof buf, in))          // 读到 EOF 返回 NULL
        printf("read: %s", buf);
    fclose(in);

    // 二进制模式："wb" / "rb"，用 fwrite/fread 整块读写
    int data[4] = {1, 2, 3, 4};
    out = fopen("/tmp/cpp-demo-bin.dat", "wb");
    fwrite(data, sizeof data[0], 4, out);
    fclose(out);
    int back[4] = {0};
    in = fopen("/tmp/cpp-demo-bin.dat", "rb");
    fread(back, sizeof back[0], 4, in);
    fclose(in);
    printf("binary: %d %d %d %d\n", back[0], back[1], back[2], back[3]);

    // 随机访问：fseek 定位，ftell 报告位置，REWIND 回到开头
    in = fopen(path, "r");
    fseek(in, 0, SEEK_END);                     // SEEK_SET/SEEK_CUR/SEEK_END
    long size = ftell(in);
    printf("file size=%ld\n", size);
    rewind(in);
    fgets(buf, sizeof buf, in);
    printf("after rewind: %s", buf);
    fclose(in);

    // stderr 是无缓冲的标准错误流；snprintf 安全格式化到缓冲区
    fprintf(stderr, "(stderr) file ok\n");
    char small[8];
    snprintf(small, sizeof small, "%s", "truncated-long-string");
    printf("snprintf: %s\n", small);

    remove(path);                               // 删除文件
    return 0;
}
