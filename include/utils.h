#ifndef __UTILS_H__
#define __UTILS_H__
#include "lib_log.h"

// 校验结果 如果不为 0 return,并输出错误
#define CHECK_RET(cond, fmt, ...)          \
    do                                     \
    {                                      \
        if (cond)                          \
        {                                  \
            LOG_ERROR(fmt, ##__VA_ARGS__); \
            return EXIT_FAILURE;           \
        }                                  \
    } while (0)

// 校验结果 如果不为 0 goto to,并输出错误
#define CHECK_RET_GOTO(cond, to, fmt, ...) \
    do                                     \
    {                                      \
        if (cond)                          \
        {                                  \
            LOG_ERROR(fmt, ##__VA_ARGS__); \
            goto to;                       \
        }                                  \
    } while (0)

#define CHECK_RET_EXIT(cond, fmt, ...)     \
    do                                     \
    {                                      \
        if (cond)                          \
        {                                  \
            LOG_ERROR(fmt, ##__VA_ARGS__); \
            exit(EXIT_FAILURE);            \
        }                                  \
    } while (0)

// 将 uint16 mac[6] 转换为字符串
#define MAC_16_TO_STR(mac_16, mac_str)                                             \
    do                                                                             \
    {                                                                              \
        sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",                          \
                mac_16[0], mac_16[1], mac_16[2], mac_16[3], mac_16[4], mac_16[5]); \
    } while (0)

// 将字符串转换为 uint16 mac[6]
#define MAC_STR_TO_MAC(mac_str, mac_16)                                                 \
    do                                                                                  \
    {                                                                                   \
        sscanf(mac_str, "%04hX:%04hX:%04hX:%04hX:%04hX:%04hX",                          \
               &mac_16[0], &mac_16[1], &mac_16[2], &mac_16[3], &mac_16[4], &mac_16[5]); \
    } while (0)

// 将 ipv4 转换为 str
#define IPV4_TO_STR(ipv4, ipv4_str)                                                         \
    do                                                                                      \
    {                                                                                       \
        sprintf(ipv4_str, "%u.%u.%u.%u",                                                    \
                (ipv4 >> 24) & 0xFF, (ipv4 >> 16) & 0xFF, (ipv4 >> 8) & 0xFF, ipv4 & 0xFF); \
    } while (0)

// 将 string 转换为 ipv4
#define STR_TO_IPV4(ipv4_str, ipv4)                                                              \
    do                                                                                           \
    {                                                                                            \
        sscanf(ipv4_str, "%u.%u.%u.%u",                                                          \
               &(ipv4 >> 24) & 0xFF, &(ipv4 >> 16) & 0xFF, &(ipv4 >> 8) & 0xFF, &(ipv4) & 0xFF); \
    } while (0)

#endif