#ifndef ETHERNET_H
#define ETHERNET_H
#include <linux/if_ether.h>

//二层数据
typedef struct eth_hdr
{
    /* data */
    unsigned char dmac[6];  //目的mac
    unsigned char smac[6];  //来源mac
    uint16_t ethertype;     //以太网类型
    unsigned char payload[];    //负载
}__attribute__((packed)) eth_hdr;

//从 sk_buff 从获取二层头部
static inline struct eth_hdr *get_eth_hdr(struct sk_buff *skb)
{
    //head 指向的是 data 的起始地址
    struct eth_hdr *hdr = (struct eth_hdr *)skb_head(skb);

    hdr->ethertype = ntohs(hdr->ethertype);
    
    return hdr;
}

eth_hdr *init_eth_hdr(char *buf);
void show_eth_hdr(eth_hdr *hdr);

#endif