// 以太网帧的转换
#include "syshead.h"
#include "basic.h"
#include "ethernet.h"

//打印头部信息
void show_eth_hdr(eth_hdr *hdr)
{
    printf("eth hdr:\n");
    printf("dmac :%s", hdr->dmac);
    printf("smac :%s", hdr->smac);
    printf("ethertype: %u", hdr->ethertype);
}

//init_eth_hdr 以太网帧头部转换
eth_hdr *init_eth_hdr(char *buf)
{
    eth_hdr *hdr = (eth_hdr *)buf;
    
    //主机字节序转换为网络字节序
    hdr->ethertype = htons(hdr->ethertype);
    show_eth_hdr(hdr);
    return hdr;
}