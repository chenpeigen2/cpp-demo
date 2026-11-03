// 07 - 结构体、联合、枚举
#include <stdio.h>
#include <stddef.h>   // offsetof
#include <stdint.h>

typedef struct {              // typedef 后可直接写 Point 而不用 struct Point
    int x, y;
} Point;

enum Color : uint8_t { RED, GREEN, BLUE };  // C23：枚举可指定底层类型

union Value {                 // 所有成员共享同一块内存，大小=最大成员
    int i;
    double d;
    char bytes[8];
};

struct Flags {                // 位域：按位分配宽度
    unsigned int a : 1;
    unsigned int b : 3;
};

int main(void) {
    // 指定初始化器（C99），未指定的成员为 0
    Point p = {.y = 3};
    printf("p=(%d,%d) size=%zu\n", p.x, p.y, sizeof p);

    // 结构体传参/赋值都是按值拷贝
    Point q = p; q.x = 100;
    printf("p.x=%d q.x=%d\n", p.x, q.x);

    // offsetof：成员相对结构体起始处的字节偏移（演示对齐/填充）
    struct Padded { char c; int i; };
    printf("offset(c)=%zu offset(i)=%zu size=%zu  // 3字节填充\n",
           offsetof(struct Padded, c), offsetof(struct Padded, i),
           sizeof(struct Padded));

    union Value v = {.d = 3.14};
    printf("union: d=%g i=%d size=%zu\n", v.d, v.i, sizeof v);

    enum Color c = BLUE;
    printf("BLUE=%d size=%zu\n", c, sizeof c);

    struct Flags f = {.a = 1, .b = 5};
    printf("bits: a=%u b=%u size=%zu\n", f.a, f.b, sizeof f);
    return 0;
}
