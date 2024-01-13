#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lib_log.h"

static uint32_t cur_debug_level = DL_1;   //当前调试日志级别

//根据 log_type 获取类型字符串
#define GET_LOG_TYPE_STR(x) \
    ((x == LT_ERROR) ? "error" : \
    (x == LT_WARN) ? "warn" : \
    (x == LT_INFO) ? "info" : "debug")

//输出日志
static void write_log(uint32_t to_type, uint32_t log_type, 
    uint32_t level, const char *file, const uint32_t lineno, 
    const char *func,const char *format, va_list ap)
{
    char* cpy_fmt = NULL;
    size_t cpy_len = strlen(format) + sizeof(file) + sizeof(func) + sizeof("[: ] ") + 4 + 1;
	cpy_fmt = malloc(cpy_len);
	if(NULL == cpy_fmt)
        return;
	
    sprintf(cpy_fmt, "[%s:%u %s %s] %s", file, lineno, func, GET_LOG_TYPE_STR(log_type), format);

    char buf[LOG_BUFF_LEN];
    memset(buf, 0, LOG_BUFF_LEN);
    vsnprintf(buf, LOG_BUFF_LEN, cpy_fmt, ap);

    // 这里你可以根据需要将日志输出到不同的地方，比如 stdout 或者 stderr TODO
    if (log_type == LT_ERROR) {
        fprintf(stderr, "%s \n", buf); // 输出到标准输出流
    } else {
        fprintf(stdout, "%s \n", buf); // 输出到标准错误流
    }

    free(cpy_fmt);
    cpy_fmt = NULL;
}

//日志输出
void log_output(uint32_t to_type, uint32_t log_type, uint32_t level,
                    const char *file, uint32_t lineno, const char *func,
                    const char *format, ...)
{
    if(log_type == LT_DEBUG && !(cur_debug_level & level)) 
    {
        return;
    }   

    va_list ap;
	va_start(ap, format);
	write_log(to_type, log_type, level, file, lineno, func, format, ap);
    va_end(ap);
}

//设置日志级别
uint32_t log_level(uint32_t level, uint8_t is_set)
{
    if(is_set)
        cur_debug_level = level;

    return cur_debug_level;
}




