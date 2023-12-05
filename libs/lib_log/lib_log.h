
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

/*
    @brief 输出日志
    @param 
        to_type 输出到那里
        log_
*/
void log_output(uint32_t to_type, uint32_t log_type, uint32_t level,
                    const char *file, uint32_t line_no, const char *func, 
                    const char *info, const char *fmt, ...)
{
    va_list ap; 
    va_start(ap, fmt);
}