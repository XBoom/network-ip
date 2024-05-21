#include "common.h"

/*
对齐准则是什么
总的来说，字节对齐有以下准则：

结构体变量的首地址能够被其对齐字节数大小所整除。
结构体每个成员相对结构体首地址的偏移都是成员大小的整数倍，如不满足，对前一个成员填充字节以满足。
结构体的总大小为结构体对最大成员大小的整数倍，如不满足，最后填充字节以满足。
*/

// demo1 代理
typedef struct demo1
{
    uint8_t a;  // 1
    uint32_t b; // 4
    uint16_t c; // 2
} demo1;

typedef struct demo2
{
    uint8_t a;  // 1
    uint16_t c; // 2
    uint32_t b; // 4
} demo2;

typedef struct demo3
{
    uint32_t b; // 4
    uint16_t c; // 2
    uint8_t a;  // 1
} demo3;

typedef struct demo4
{
    uint8_t a;  // 1
    uint32_t b; // 4
    uint16_t c; // 2
    uint8_t d;  // 1
    uint32_t e; // 4
    uint16_t f; // 2
} demo4;

typedef struct demo5
{
    uint8_t a;  // 1
    uint16_t c; // 2
    uint32_t b; // 4
    uint8_t d;  // 1
    uint16_t e; // 2
    uint32_t f; // 4
} demo5;

typedef struct demo6
{
    uint32_t b; // 4
    uint16_t c; // 2
    uint8_t a;  // 1
    uint32_t d; // 4
    uint16_t e; // 2
    uint8_t f;  // 1
} demo6;

int main(int argc, char *argv[])
{
    printf("%d %d %d", (int)sizeof(demo1), (int)sizeof(demo2), (int)sizeof(demo3));
    printf("%d %d %d", (int)sizeof(demo4), (int)sizeof(demo5), (int)sizeof(demo6));
    return 0;
}