#include "common.h"

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server <ip>");

    struct hostent* addr_info;
    struct sockaddr_in host_addr;
    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_addr.s_addr = inet_addr(argv[1]);
    CHECK_RET(host_addr.sin_addr.s_addr == INADDR_NONE, "inet_addr failed");

    addr_info = gethostbyaddr((char *)&host_addr.sin_addr, 4, AF_INET);
    CHECK_RET(addr_info == NULL, "gethostbyaddr failed %s", hstrerror(h_errno));

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
    return 0;
    
}