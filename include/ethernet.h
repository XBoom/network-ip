#ifndef ETHERNET_H
#define ETHERNET_H
#include <linux/if_ether.h>

//二层数据头
typedef struct eth_hdr
{
    /* data */
    unsigned char dmac[6];  //目的mac
    unsigned char smac[6];  //来源mac
    uint16_t ethertype;     //以太网类型
    unsigned char payload[];    //负载
}__attribute__((packed)) eth_hdr;

#endif