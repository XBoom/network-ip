#ifndef __LIB_LOG_H__
#define __LIB_LOG_H__

#define LOG_BUFF_LEN 512

// 日志输出到哪里
#define TO_ALL 0xff      // 输出日志到所有
#define TO_TERM 0x0001   // 输出日志到终端
#define TO_LOGS 0x0002   // 输出日志到logs
#define TO_TELNET 0x0004 // 输出日志到telnet
#define TO_FILE 0x0008   // 输出日志到文件

// 调剂日志级别
#define DL_NUM (32) // 32个日志调试级别
#define DL_1 (0x1)
#define DL_2 (0x1 << 1)
#define DL_3 (0x1 << 2)
#define DL_4 (0x1 << 3)
#define DL_5 (0x1 << 4)
#define DL_6 (0x1 << 5)
#define DL_7 (0x1 << 6)
#define DL_8 (0x1 << 7)
#define DL_9 (0x1 << 8)
#define DL_10 (0x1 << 9)
#define DL_11 (0x1 << 10)
#define DL_12 (0x1 << 11)
#define DL_13 (0x1 << 12)
#define DL_14 (0x1 << 13)
#define DL_15 (0x1 << 14)
#define DL_16 (0x1 << 15)
#define DL_17 (0x1 << 16)
#define DL_18 (0x1 << 17)
#define DL_19 (0x1 << 18)
#define DL_20 (0x1 << 19)
#define DL_21 (0x1 << 20)
#define DL_22 (0x1 << 21)
#define DL_23 (0x1 << 22)
#define DL_24 (0x1 << 23)
#define DL_25 (0x1 << 24)
#define DL_26 (0x1 << 25)
#define DL_27 (0x1 << 26)
#define DL_28 (0x1 << 27)
#define DL_29 (0x1 << 28)
#define DL_30 (0x1 << 29)
#define DL_31 (0x1 << 30)
#define DL_32 (0x1 << 31)

#define LT_ERROR (1) // 错误日志
#define LT_WARN (2)  // 告警日志
#define LT_DEBUG (3) // 调试日志
#define LT_INFO (4)  // 信息日志

void log_output(uint32_t to_type, uint32_t log_type, uint32_t level,
                    const char *file, uint32_t lineno, const char *func,
                    const char *format, ...);

uint32_t log_level(uint32_t level, uint8_t is_set);

#define WRITE_LOG(to_type, log_type, level, format, ...) \
    do                                                   \
    {                                                    \
        log_output(to_type, log_type, level,         \
                       __FILE__, __LINE__, __func__,     \
                       format, ##__VA_ARGS__);           \
    } while (0)

// 默认写到屏幕
#define LOG_ERROR(format, ...) \
    WRITE_LOG(TO_TERM, LT_ERROR, 0, format, ##__VA_ARGS__)

#define LOG_WARN(format, ...) \
    WRITE_LOG(TO_TERM, LT_WARN, 0, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
    WRITE_LOG(TO_TERM, LT_INFO, 0, format, ##__VA_ARGS__)

#define LOG_DEBUG(level, format, ...) \
    WRITE_LOG(TO_TERM, LT_DEBUG, level, format, ##__VA_ARGS__)

// fprintf(file_pointer, "格式化字符串", 变量1, 变量2, ...);
//   FILE *stream：指向 FILE 对象的指针，表示要写入的文件流
//       stdout 标准输出，这样输出就会显示在屏幕上(printf 默认)
//       stderr 标准错误输出。它通常用于显示错误消息和诊断信息，也会显示在屏幕上
#define REAL_INFO(format, ...)                          \
    do                                              \
    {                                               \
        fprintf(stdout, "[%s:%d]" format "\n",         \
                __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define REAL_ERROR(format, ...)                         \
    do                                              \
    {                                               \
        fprintf(stderr, "[%s:%d]" format "\n",         \
                __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define LOG_SET_LEVEL(x) log_level(x, 1);
#define LOG_GET_LEVEL() log_level(0, 0)

#endif