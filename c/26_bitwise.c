// 26 - 位运算技巧与 <limits.h>（C23 <stdbit.h> 在 macOS SDK 缺失，手写等价函数）
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

// popcount：统计 1 的个数（Brian Kernighan 算法：每次消掉最低位的 1）
static int popcount32(uint32_t x) {
    int n = 0;
    while (x) { x &= x - 1; n++; }
    return n;
}

// 检查是否为 2 的幂：2 的幂只有最高位是 1
static _Bool is_pow2(uint32_t x) { return x && !(x & (x - 1)); }

// 向上取整到 2 的幂
static uint32_t next_pow2(uint32_t x) {
    x--;
    x |= x >> 1;  x |= x >> 2;  x |= x >> 4;
    x |= x >> 8;  x |= x >> 16;
    return x + 1;
}

int main(void) {
    // 掩码与位段操作：第 n 位置 1 / 清 0 / 翻转 / 测试
    uint32_t flags = 0;
    flags |= 1u << 3;                    // 置第 3 位
    printf("set bit3=%u test=%d\n", flags, (flags >> 3) & 1u);
    flags &= ~(1u << 3);                 // 清第 3 位
    printf("clear bit3=%u\n", flags);
    flags ^= 1u << 5;                    // 翻转第 5 位
    printf("toggle bit5=%u\n", flags);

    // 经典技巧
    printf("popcount(0b1011)=%d is_pow2(64)=%d is_pow2(65)=%d\n",
           popcount32(0b1011), is_pow2(64), is_pow2(65));
    printf("next_pow2(100)=%u\n", next_pow2(100));

    // 取最低位的 1：x & -x
    uint32_t v = 0b110100;
    printf("lowest set bit=%u\n", v & (uint32_t)(-(int32_t)v));

    // 交换两个值不用临时变量（异或性质，纯演示；实际写 a^=b 会更难读）
    int a = 3, b = 9;
    a ^= b; b ^= a; a ^= b;
    printf("swap: a=%d b=%d\n", a, b);

    // 符号扩展与位域宽度：CHAR_BIT 报告每字节位数
    printf("CHAR_BIT=%d UINT_MAX=%u\n", CHAR_BIT, UINT_MAX);
    return 0;
}
