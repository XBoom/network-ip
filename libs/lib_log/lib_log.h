#ifndef __LIB_LOG_H__
#define __LIB_LOG_H__

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
    va_list ap; //可以在函数内容访问可变参数列表中的参数
    char buf[CMDBUFLEN];
    va_start(ap, cmd);  //ap初始化为可变参数的起始位置
    vsnprintf(buf, CMDBUFLEN, cmd, ap);  //格式化可变参数
    va_end(ap);     //清理 va_list
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