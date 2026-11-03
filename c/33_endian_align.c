// 33 - 实战：字节序、对齐与填充（二进制协议和 ABI 的地基）
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#pragma pack(push, 1)
typedef struct {                 // pack(1)：彻底关闭填充
    char a;
    int  i;                      // 紧贴 a，不填充
    char b;
} Packed;                        // 1+4+1 = 6 字节
#pragma pack(pop)

typedef struct { char a; int i; char b; } Natural;   // 自然对齐：12 字节

int main(void) {
    // ---- 字节序：多字节整数在内存里的摆放顺序 ----
    union { uint32_t u; uint8_t b[4]; } probe = {.u = 0x01020304};
    printf("0x01020304 的第 1 个字节 = 0x%02X → %s\n", probe.b[0],
           probe.b[0] == 0x04 ? "小端(低字节在低地址, x86/ARM)" : "大端(网络字节序)");
    // 手动转换（等价 ntohl/htonl）
    uint32_t v = 0x01020304;
    uint32_t rev = ((v & 0xFF) << 24) | ((v & 0xFF00) << 8) | ((v >> 8) & 0xFF00) | (v >> 24);
    printf("手动反转字节 = 0x%08X\n", rev);

    // ---- 对齐：_Alignas 指定强对齐（SIMD/缓存行场景）----
    _Alignas(64) char cacheline[64];
    printf("_Alignas(64) 地址 64 对齐? %d（缓存行友好）\n",
           ((uintptr_t)cacheline % 64) == 0);
    printf("alignof: char=%zu int=%zu double=%zu long double=%zu max_align_t=%zu\n",
           _Alignof(char), _Alignof(int), _Alignof(double),
           _Alignof(long double), _Alignof(max_align_t));

    // ---- 填充：成员按自身对齐值摆放，中间留空洞 ----
    printf("\n自然对齐布局（成员偏移）：\n");
    printf("  Natural: a@%zu i@%zu b@%zu size=%zu（b 后面填 3 字节凑 4 的倍数）\n",
           offsetof(Natural, a), offsetof(Natural, i), offsetof(Natural, b),
           sizeof(Natural));
    printf("  pack(1): a@%zu i@%zu b@%zu size=%zu（无空洞，但跨字节访问更慢，ABI 不通用）\n",
           offsetof(Packed, a), offsetof(Packed, i), offsetof(Packed, b),
           sizeof(Packed));

    // 成员顺序影响大小：同样的字段，不同的排列
    struct { char a, b; int i; char c; } good;      // 2+2填+4+1+3填 = 12? 实际 12
    struct { char a; int i; char b, c; } better;    // 1+3填+4+1+1 = 8
    printf("\n字段排序对比: {c,c,int,c}=%zu 字节  {c,int,c,c}=%zu 字节（把小成员凑一起）\n",
           sizeof good, sizeof better);

    // 对齐是 ABI 的一部分：跨进程/网络传结构体，两端 pack 必须一致，
    // 或者干脆逐字段序列化（这也是 protobuf 等库存在的理由）
    return 0;
}
