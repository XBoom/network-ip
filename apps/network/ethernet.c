// 以太网帧的转换
#include "syshead.h"
#include "basic.h"
#include "ethernet.h"

#define ETH_ADDR_LEN (6)
//打印头部信息
void show_eth_hdr(struct eth_hdr *hdr)
{
    printf("eth hdr:\n");
    printf("dmac: ");
    for (int i = 0; i < ETH_ADDR_LEN; i++) {
        printf("%02X ", hdr->dmac[i]);
    }
    printf("\n");

    printf("smac: ");
    for (int i = 0; i < ETH_ADDR_LEN; i++) {
        printf("%02X ", hdr->smac[i]);
    }
    printf("\n");

    printf("ethertype: %04X \n", hdr->ethertype);
}

//init_eth_hdr 以太网帧头部转换
struct eth_hdr *init_eth_hdr(char *buf)
{
    struct eth_hdr *hdr = (struct eth_hdr *)buf;
    
    //主机字节序转换为网络字节序
    hdr->ethertype = htons(hdr->ethertype);
    show_eth_hdr(hdr);
    return hdr;
}