#include "common.h"

int main(int argc, char *argv[])
{
    //整数本地字节序转网络字节序
    uint16_t host_port= 0x1234;
    uint16_t net_port = 0;
    uint32_t host_addr = 0x12345678;
    uint32_t net_addr = 0;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    //#：表示在输出八进制或十六进制数时，要加上相应的前缀（0、0x）。
    //x：表示以十六进制形式输出整数。
    LOG_INFO("host ordered port: %#x", host_port);
    LOG_INFO("net ordered addr: %#x", net_port);
    LOG_INFO("host ordered post: %#lx", host_addr);
    LOG_INFO("net ordered post: %#lx", host_addr);

    //inet_addr 字符串地址转网络字节序
    char *addr1 = "127.0.0.1";
    char *addr2 = "127.0.0.258";
    net_addr = inet_addr(addr1);
    if(net_addr == INADDR_NONE)
    {
        LOG_ERROR("inet addr %s failed", addr1);
    }
    else 
    {
        LOG_INFO("inet addr %s success %#x", addr1, net_addr);
    }

    net_addr = inet_addr(addr2);
    if(net_addr == INADDR_NONE)
    {
        LOG_ERROR("inet addr %s failed", addr2);
    }
    else 
    {
        LOG_INFO("inet addr %s success %#x", addr2, net_addr);
    }

    //地址直接转换
    struct sockaddr_in addr_sock;

    if(!inet_aton(addr1, (struct in_addr *)&addr_sock.sin_addr))
    {
        LOG_ERROR("inet_aton %s failed", addr1);
    }
    else
    {
        LOG_INFO("inet_aton %s success %#lx", addr1, addr_sock.sin_addr.s_addr);
    }

    if(!inet_aton(addr2, (struct in_addr *)&addr_sock.sin_addr))
    {
        LOG_ERROR("inet_aton %s failed", addr2);
    }
    else
    {
        LOG_INFO("inet_aton %s success %#lx", addr2, addr_sock.sin_addr.s_addr);
    }

    //网络字节序转地址
    struct sockaddr_in addr_sock2;
    addr_sock2.sin_addr.s_addr = inet_addr(addr1);
    char * addr1_back = inet_ntoa((struct in_addr)addr_sock2.sin_addr);
    LOG_INFO("inet ntoa %s %s", addr1, addr1_back);

    return 0;
}