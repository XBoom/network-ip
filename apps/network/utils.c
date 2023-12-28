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

/**
 *  计算16bit合
 *  addr 被计算目标
 *  count 字节数
 * */ 
uint32_t sum_every_16bits(void *addr, int count)
{
    register uint32_t sum = 0;
    uint16_t * ptr = addr;

    while(count > 1)
    {
        sum += * ptr++;
        count -= 2; //因为ptr是两个字节，所以没计算一次就 -2
    }

    //如果还有一个字节，那么将这个字节也加进去
    if(count > 0)
        sum += * (uint8_t *)ptr;
    
    return sum;
}

/**
 * 校验和
 * addr 被计算目标
 * count 字节数
 * start_sumn 起始合
*/
uint16_t checksum(void *addr, int count, int start_sum)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */

    uint32_t sum = start_sum;
    sum += sum_every_16bits(addr, count);

    while(sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;

}