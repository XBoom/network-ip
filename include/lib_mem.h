#ifndef __LIB_MEM_H__
#define __LIB_MEM_H__
#include <stdlib.h>

#define X_MALLOC(x) malloc(x)
#define X_CLEAR(x) memset(&(x), 0, sizeof(x))
#define X_CALLOC(X) calloc(1, X)

#define X_FREE(x)     \
    do                \
    {                 \
        if (x)        \
        {             \
            free(x);  \
            x = NULL; \
        }             \
    } while (0)

// 内存池结构

// 初始化内存池

// 释放内存池

// 分配一个指定大小的内存

// 重新分配一个更大的内存

#endif