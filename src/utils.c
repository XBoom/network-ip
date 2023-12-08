#include "syshead.h"
#include "utils.h"

extern int debug_switch;    //调试开关

//run_cmd 执行命令
int run_cmd(char *cmd, ...)
{
    va_list ap; //可以在函数内容访问可变参数列表中的参数
    char buf[CMDBUFLEN];
    va_start(ap, cmd);  //ap初始化为可变参数的起始位置
    vsnprintf(buf, CMDBUFLEN, cmd, ap);  //格式化可变参数
    va_end(ap);     //清理 va_list

    if(debug_switch)
        printf("exec: %s\n", buf);

    return system(buf); //执行命令
}

//计算给定内存块中每16位数据之和的函数
uint32_t sum_every_16bits(void *addr, int count)
{
    register uint32_t sum = 0;  //使用 register 关键字声明寄存器变量 sum，用于保存计算结果的总和
    uint16_t * ptr = addr;
    
    while( count > 1 )  {
        sum += * ptr++;
        count -= 2; //因为每次处理了两个字节的数据
    }

    //如果剩余的字节数为1（即 count 大于0），则将剩余的一个字节的数据转换为 uint8_t 类型，并加到 sum 中
    if( count > 0 )
        sum += * (uint8_t *) ptr;

    return sum;
}

//计算校验和
uint16_t checksum(void *addr, int count, int start_sum)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */
    uint32_t sum = start_sum;

    sum += sum_every_16bits(addr, count);
    
    /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

//获取地址
int get_address(char *host, char *port, struct sockaddr *addr)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(host, port, &hints, &result);

    if (s != 0) {
        print_err("getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        *addr = *rp->ai_addr;
        freeaddrinfo(result);
        return 0;
    }
    
    return 1;
}

//转换地址
uint32_t parse_ipv4_string(char* addr) {
    uint8_t addr_bytes[4];
    sscanf(addr, "%hhu.%hhu.%hhu.%hhu", &addr_bytes[3], &addr_bytes[2], &addr_bytes[1], &addr_bytes[0]);
    return addr_bytes[0] | addr_bytes[1] << 8 | addr_bytes[2] << 16 | addr_bytes[3] << 24;
}

uint32_t min(uint32_t x, uint32_t y) {
    return x > y ? y : x;
}