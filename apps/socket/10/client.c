#include "common.h"

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: client <hostname>");

    int ret = 0;
    struct hostent* addr_info;

    addr_info = gethostbyname(argv[1]);
    CHECK_RET(addr_info == NULL, "gethostbyname failed");

    LOG_INFO("%s get host name:", argv[1]);
    LOG_INFO("\th_name:%s", addr_info->h_name);
    LOG_INFO("\th_aliases:");
    for(int i = 0; addr_info->h_aliases[i]; i++)
    {
        LOG_INFO("\t\t[%d]:%s", i, addr_info->h_aliases[i]);
    }
    LOG_INFO("\t type:%s", addr_info->h_addrtype == AF_INET ? "IPv4":"IPv6");
    LOG_INFO("\t h_length:%d", addr_info->h_length);
    LOG_INFO("\t h_addr_list:");
    for(int i = 0; addr_info->h_addr_list[i]; i++)
    {
        LOG_INFO("\t\t[%d]:%s",i, inet_ntoa(*(struct in_addr *)addr_info->h_addr_list[i]));
    }


    //getaddrinfo
    struct addrinfo addr_info2, *result;
    memset(&addr_info2, 0, sizeof(addr_info2));
    addr_info2.ai_family = AF_INET;
    addr_info2.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(argv[1], NULL, &addr_info2, &result);
    CHECK_RET(ret != 0, "get addr info failed %s", hstrerror(h_errno));

    LOG_INFO("\ngetaddrinfo:");
    struct addrinfo* p;
    void * addr;
    for(p = result; p; p = p->ai_next)
    {
        // 获取地址
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // 将地址转换为字符串表示
        char ipstr[1024] = {0};
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        LOG_INFO("\t ip: %s", ipstr);
    }

    return 0;
    
}