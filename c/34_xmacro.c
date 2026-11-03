// 34 - 实战：X-macro 元编程 —— 一份数据，生成枚举/字符串/字段表
#include <stdio.h>
#include <stddef.h>

// ---- 用法一：枚举 ↔ 字符串（协议/日志必备）----
#define COLORS(X) \
    X(RED)        \
    X(GREEN)      \
    X(BLUE)       \
    X(ORANGE)     \
    X(PURPLE)

#define AS_ENUM(c) COLOR_##c,
enum Color { COLORS(AS_ENUM) };          // 展开成 COLOR_RED, COLOR_GREEN, ...

#define AS_STR(c) #c,
static const char *const color_names[] = { COLORS(AS_STR) };   // "RED", "GREEN", ...

#define AS_HEX(c) case COLOR_##c: return "#<" #c ">";
static const char *color_hex(enum Color c) {
    switch (c) { COLORS(AS_HEX) }
    return "?";
}

// ---- 用法二：结构体字段自描述（手搓反射/序列化）----
#define FIELDS(X)     \
    X(int,    id)     \
    X(double, score)  \
    X(char,   grade)

#define DECL(t, n) t n;
struct Record { FIELDS(DECL) };          // 展开成 int id; double score; char grade;

#define FIELD_DESC(t, n) { #t, #n, offsetof(struct Record, n), sizeof(((struct Record *)0)->n) },
static const struct { const char *type, *name; size_t off, size; } field_table[] = {
    FIELDS(FIELD_DESC)
};

int main(void) {
    // 枚举与名字数组天然同序
    for (int c = COLOR_RED; c <= COLOR_PURPLE; c++)
        printf("Color[%d] = %-8s %s\n", c, color_names[c], color_hex(c));
    printf("枚举个数 = %zu（加一个 X(COLOR) 行，三处代码自动同步）\n\n",
           sizeof color_names / sizeof color_names[0]);

    // 字段表：打印每个字段的类型/名字/偏移/大小
    printf("%-8s %-8s 偏移  大小\n", "类型", "字段");
    for (unsigned long i = 0; i < sizeof field_table / sizeof field_table[0]; i++)
        printf("%-8s %-8s %4zu  %4zu\n", field_table[i].type, field_table[i].name,
               field_table[i].off, field_table[i].size);

    // 这张表就是"穷人版反射"：通用打印、JSON 序列化、diff 都可以用它驱动
    struct Record r = {.id = 7, .score = 92.5, .grade = 'A'};
    printf("\n表驱动打印: id=%d score=%.1f grade=%c\n", r.id, r.score, r.grade);
    return 0;
}
