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

struct sockaddr
{
    sa_family_t sin_family; //协议族
    char sa_data[14];   //地址信息
}

unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long ntohl(unsigned long);
unsigned long ntohl(unsigned long);

#include <arpa/inet.h>
//成功时返回32位大端序整数型值，失败返回INADDR_NONE(检查无效IP地址)
in_addr_t inet_adddr(const char * string);

//与上面功能一样，成功返回1，失败返回0
int inet_aton(const char * string, struct in_addr *addr);
//将网络字节序地址转换为字符串形式(注意返回结果的释放)
char *inet_ntoa(struct in_addr *addr);


