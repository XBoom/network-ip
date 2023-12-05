/*
    工具
*/
#include "syshead.h"
#include "utils.h"

//run_cmd 执行命令
int run_cmd(char *cmd, ...)
{

    va_list ap; //可以在函数内容访问可变参数列表中的参数
    char buf[CMDBUFLEN];
    va_start(ap, cmd);  //ap初始化为可变参数的起始位置
    vsnprintf(buf, CMDBUFLEN, cmd, ap);  //格式化可变参数
    va_end(ap);     //清理 va_list

    printf("%s\n", buf);

    return system(buf); //执行命令
}

//打印字符串的16进制
void print_hexdump(char *str, int len)
{
    printf("Printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 8 == 0) printf("\n");
        printf("%02x ", (unsigned char)str[i]);
    }

    printf("\n");
}

//打印错误
void print_error(char *str, ...)
{
    va_list ap;
    char buf[200];
    va_start(ap, str);
    vsnprintf(buf, 200, str, ap);

    va_end(ap);

    perror(buf);
}

//校验和
uint16_t checksum(void *addr, int count, int start_sum)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */
    return 0;
}