#ifndef ETHERNET_H
#define ETHERNET_H
#include <linux/if_ether.h>
#include "skbuff.h"
#include "syshead.h"

//二层接口

#define ETH_HDR_LEN sizeof(struct eth_hdr) //二层头

struct sk_buff;
struct netdev;

uint8_t *skb_head(struct sk_buff *skb);

//二层数据
struct eth_hdr
{
    /* data */
    uint8_t dmac[6];  //目的mac
    uint8_t smac[6];  //来源mac
    uint16_t ethertype;     //以太网类型
    uint8_t payload[];    //负载
}__attribute__((packed));

//从 sk_buff 从获取二层头部
static inline struct eth_hdr *get_eth_hdr(struct sk_buff *skb)
{
    //head 指向的是 data 的起始地址
    struct eth_hdr *hdr = (struct eth_hdr *)skb_head(skb);

    hdr->ethertype = ntohs(hdr->ethertype);
    
    return hdr;
}

struct eth_hdr *init_eth_hdr(char *buf);
void show_eth_hdr(struct eth_hdr *hdr);

#endif