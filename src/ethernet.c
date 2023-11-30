// 以太网帧的转换
#include "syshead.h"
#include "basic.h"
#include "ethernet.h"

//init_eth_hdr 以太网帧头部转换
eth_hdr *init_eth_hdr(char *buf)
{
    eth_hdr *hdr = (eth_hdr *)buf;
    
    //主机字节序转换为网络字节序
    hdr->ethertype = htons(hdr->ethertype);
    return hdr;
}