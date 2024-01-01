#include "common.h"

struct sockaddr_in
{
    sa_family_t sin_family;     //地址族(Address Family) __uint8_t
    uint16_t sin_port;          //16 位 TCP/UDP 端口号
    struct in_addr sin_addr;    //32 位 IP 地址
    char   sin_zero[8];    //不使用
}

struct in_addr
{
    in_addr_t s_addr;   //32 位 IPv4 地址 (__uint32_t)  
}