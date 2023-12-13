#ifndef __LIB_LOG_H__
#define __LIB_LOG_H__

#define LOG_BUFF_LEN 1024

//log_fmt_t 日志结构
typedef struct log_fmt_t
{
    uint32_t log_type;  //日志类型
    const char *file;   //文件名
    const char *func;   //函数名
    uint32_t line_no;   //行号
    const char *info;   //前缀信息
    const char *fmt;    //日志格式
    va_list args;       //可变参数
}log_fmt_t;

//输出日志
void log_output(uint32_t to_type, uint32_t log_type, 
    uint32_t log_level, const char *info, const char *fmt, ...)
{
    va_list ap;
    char buf[LOG_BUFF_LEN];
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUFF_LEN, fmt, ap);
    va_end(ap);

    // 这里你可以根据需要将日志输出到不同的地方，比如 stdout 或者 stderr
    if (to_type == 1) {
        fprintf(stdout, "%s \n", buf); // 输出到标准输出流
    } else {
        fprintf(stderr, "%s \n", buf); // 输出到标准错误流
    }
}

//输出到tcp
// static void tcp_printf(cl_tcp *tcp, const char* fmt, ...)
// {
// #ifdef CTEST    //如果是单元测试，直接退出
//     return;
// #endif
//     va_list ap;
//     char buf[DEFAULT_WRITE_SIZE];
//     memset(buf, 0, DEFAULT_WRITE_SIZE);

//     va_start(ap, fmt);
//     vsprintf_n(buf, DEFAULT_WRITE_SIZE, fmt, ap);
//     va_end(ap);

//     buf[DEFAULT_WRITE_SIZE - 1] = '\0';

//     //cl_tcp_syn_write(tcp, buf, strlen(buf));
// }


//fprintf(file_pointer, "格式化字符串", 变量1, 变量2, ...);
//  FILE *stream：指向 FILE 对象的指针，表示要写入的文件流
//      stdout 标准输出，这样输出就会显示在屏幕上(printf 默认)
//      stderr 标准错误输出。它通常用于显示错误消息和诊断信息，也会显示在屏幕上
#define LOG_INFO(fmt,...) \
do { \
    fprintf(stdout, "[%s:%d]"fmt"\n", \
                                 __func__, __LINE__, ##__VA_ARGS__); \
} while(0)
#define LOG_ERROR(fmt,...) \
do { \
    fprintf(stderr, "[%s:%d]"fmt"\n", \
                                 __func__, __LINE__, ##__VA_ARGS__); \
} while(0)

#endif