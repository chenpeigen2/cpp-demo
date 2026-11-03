// 18 - <stdint.h> <inttypes.h> <limits.h> <float.h>：整数与浮点的边界
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

int main(void) {
    // 固定宽度整数：精确控制大小，跨平台代码必备
    int8_t   a = INT8_MAX;      //  127
    uint16_t b = UINT16_MAX;    // 65535
    int32_t  c = INT32_MAX;     // 约 21 亿
    int64_t  d = INT64_MAX;
    printf("i8=%" PRId8 " u16=%" PRIu16 " i32=%" PRId32 "\n", a, b, c);
    printf("i64=%" PRId64 "\n", d);            // inttypes.h 提供跨平台格式宏
    printf("i64 hex=%" PRIX64 "\n", (uint64_t)d);

    // 最快/最小宽度类型与指针宽度类型
    printf("fast i32 size=%zu intptr fits pointer=%zu\n",
           sizeof(int_fast32_t), sizeof(intptr_t));

    // 溢出是 UB，边界检查可用边界值判断
    printf("INT_MAX=%d CHAR_BIT=%d\n", INT_MAX, CHAR_BIT);

    // 浮点边界
    printf("DBL_DIG=%d DBL_MAX=%g DBL_MIN=%g FLT_EPSILON=%g\n",
           DBL_DIG, DBL_MAX, DBL_MIN, FLT_EPSILON);

    // 有符号溢出示意：把 INT_MAX 转成无符号再加（无符号回绕是良定义的）
    printf("UINT wrap: %u\n", (unsigned)INT_MAX + 2u);
    return 0;
}
