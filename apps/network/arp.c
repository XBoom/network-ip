#include "arp.h"
#include "netdev.h"
#include "skbuff.h"
#include "list.h"
#include "utils.h"

/*
 * https://tools.ietf.org/html/rfc826
 */

//全局缓存
static arp_cache_entry arp_cache[ARP_CACHE_LEN];

//插入arp表
static int insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    arp_cache_entry *entry;
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {
        entry = &arp_cache[i];

        if(entry->state == ARP_FREE)
        {
            entry->state = ARP_RESOLVED;

            entry->hwtype = hdr->hwtype;
            entry->sip = data->sip;
            memcpy(entry->smac, data->smac, sizeof(sizeof(entry->smac)));

            return 0;
        }
    }
    return -1;
}

// 更新 arp 表
static int update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    arp_cache_entry *entry;
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {
        entry = &arp_cache[i];

        if(entry->state == ARP_FREE) continue;

        if(entry->hwtype == hdr->hwtype && entry->sip == data->sip)
        {
            memcpy(entry->smac, data->smac, 6);
            return 1;
        }
    }
    return 0;
}

//arp 初始化
void arp_init()
{
    memset(arp_cache, 0, ARP_CACHE_LEN * sizeof(arp_cache_entry));
}

/**
 * 响应ARP请求
 * skb ARP 请求报文
 * dev 网络设备
*/
void arp_reply(struct sk_buff *skb, struct netdev *dev)
{
    struct arp_ipv4 *arp_d;
    struct arp_hdr *arp_h;

    arp_h = arp_hdr_init(skb);  //获取arp 请求头
    
    // 预留ARP 数据的内容
    skb_reserve(skb, ETH_HDR_LEN + ARP_HDR_LEN + ARP_IPV4_LEN);

    // 设置 arp_hdr + arp_ipv4 的起始位置
    skb_push(skb, ARP_HDR_LEN + ARP_IPV4_LEN);

    //设置 arp 响应
    arp_d = (struct arp_ipv4 *)arp_h->data;

    //目的设备
    memcpy(arp_d->dmac, arp_d->smac, 6);
    arp_d->dip = arp_d->sip;

    //来源设备
    memcpy(arp_d->smac, arp_d->dmac, 6);
    arp_d->sip = arp_d->dip;

    arp_h->opcode = ARP_REPLY;
    arp_h->opcode = htons(arp_h->opcode);
    arp_h->hwtype = htons(arp_h->hwtype);
    arp_h->protype= htons(arp_h->protype);

    skb->dev = dev; //指定发送设备

    netdev_transmit(skb, arp_d->dmac, ETH_P_ARP);

    free_skb(skb);
}

//接受 ARP
void arp_receive(struct sk_buff *skb)
{
    struct arp_hdr *arp_h = arp_hdr_init(skb);
    struct arp_ipv4 *arp_d;
    struct netdev *dev;
    int merge = 0;

    //根据头部进行转换
    arp_h->hwtype = ntohs(arp_h->hwtype);
    arp_h->protype = ntohs(arp_h->protype);
    arp_h->opcode = ntohs(arp_h->opcode);

    //校验
    if(arp_h->hwtype != ARP_ETHERNET) //0x0001
    {
        printf("not support hw type");
        return;
    }

    if(arp_h->protype != ARP_IPV4) //0x0800
    {
        printf("not support proto type");
        return;
    }

    arp_d = (struct arp_ipv4 *)arp_h->data;
    //ntohl 用于将 32 位的无符号整数从网络字节顺序转换为主机字节顺序
    //ntohs 用于将 16 位的无符号整数从网络字节顺序转换为主机字节顺序。
    arp_d->dip = ntohl(arp_d->dip);
    arp_d->sip = ntohl(arp_d->sip);
    show_arp_ipv4(arp_d);

    //更新 arp 表
    merge = update_arp_translation_table(arp_h, arp_d);

    if(!(dev = netdev_get(arp_d->dip)))
    {
        printf("no found %u net dev", arp_d->dip);
        goto drop_skb;
    }

    if(!merge && insert_arp_translation_table(arp_h, arp_d) != 0)
    {
        printf("ERR: No free space in ARP translation table\n");
        goto drop_skb;
    }

    switch (arp_h->opcode)
    {
        case ARP_REQUEST:   //0x0001
            arp_reply(skb, dev);
            return;
        default:
            printf("ARP: Opcode not supported\n");
            goto drop_skb;
    }
drop_skb:
    free(skb);
    return;
}

//显示 arp ipv4 内容
void show_arp_ipv4(struct arp_ipv4 * arp_d)
{
    char src_ip[MAX_IP_LEN] = {0};
    char dst_ip[MAX_IP_LEN] = {0};
    char src_mac[MAX_MAC_STR_LEN] = {0};
    char dst_mac[MAX_MAC_STR_LEN] = {0};
    IPV4_TO_STR(arp_d->sip, src_ip);
    IPV4_TO_STR(arp_d->dip, dst_ip);
    MAC_16_TO_STR(arp_d->smac, src_mac);
    MAC_16_TO_STR(arp_d->dmac, dst_mac);
    printf("sip:%s, smac:%s, dip:%s, dmac:%s \n", 
        src_ip, src_mac, dst_ip, dst_mac);
}