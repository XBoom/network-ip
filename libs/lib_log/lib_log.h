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
void log_output(uint32_t to_type, uint32_t log_type, uint32_t level,
                    const char *file, uint32_t line_no, const char *func, 
                    const char *info, const char *fmt, ...)
{
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

#endif