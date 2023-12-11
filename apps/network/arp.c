#include "arp.h"
#include "netdev.h"
#include "skbuff.h"
#include "list.h"

/*
 * https://tools.ietf.org/html/rfc826
 */

//全局缓存
static arp_cache_entry arp_cache[ARP_CACHE_LEN];

//插入arp表
static int insert_arp_translation_table(arp_hdr *hdr, arp_ipv4 *data)
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
static int update_arp_translation_table(arp_hdr *hdr, arp_ipv4 *data)
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

//接收 ARP
void arp_incoming(netdev *dev, eth_hdr *hdr)
{
    arp_hdr *arp_h;
    arp_ipv4 *arp_d;
    int merge = 0;

    //由于是((pack)),直接进行转换
    arp_h = (arp_hdr *) hdr->payload;

    //网络字节序转为本地字节序(16bit)
    arp_h->hwtype = ntohs(arp_h->hwtype);
    arp_h->protype = ntohs(arp_h->protype);
    arp_h->opcode = ntohs(arp_h->opcode);

    if(arp_h->hwtype != ARP_ETHERNET)
    {
        printf("not support hw type \n");
        return;
    }

    if(arp_h->protype != ARP_IPV4)
    {
        printf("not support hw type \n");
        return;
    }

    //直接将 body 转换
    arp_d = (arp_ipv4 *)arp_h->data;


    //更新 arp 表
    merge = update_arp_translation_table(arp_h, arp_d);

    //如何设备的 ip 地址，与 ARP 请求的 mac 地址不一致，则表示 arp 不是这个接口的
    if(dev->addr != arp_d->dip)
    {
        printf("arp was not for us \n");
    }

    //如果是新的则插入
    if(!merge && insert_arp_translation_table(arp_h, arp_d) != 0)
    {
        perror("no free space in arp translation table \n");
    }

    //根据 arp 类型处理
    switch (arp_h->opcode)
    {
    case ARP_REQUEST:
        arp_reply(dev, hdr, arp_h);
        break;
    default:
        printf("opcode[%x] not supported \n", arp_h->opcode);
        break;
    }
}

/**
 * 响应ARP请求
 * skb ARP 请求报文
 * dev 网络设备
*/
void arp_reply(struct sk_buff *skb, netdev *dev)
{
    struct arp_ipv4 *arp_d;
    struct arp_hdr *arp_h;

    arp_h = arp_hdr_init(skb);  //获取arp 请求头
    
    // TODO 
    skb_reserve(skb, ETH_HDR_LEN + ARP_HDR_LEN + ARP_DATA_LEN);
    skb_push(skb, ARP_HDR_LEN + ARP_DATA_LEN);

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

    netdev_transmit(skb, arpdata->dmac, ETH_P_ARP);

    free_skb(skb);
}

//接受 ARP
void arp_receive(struct sk_buff * skb)
{
    arp_hdr *arp_h = arp_hdr_init(skb->head);
    arp_ipv4 *arp_d;
    netdev *dev;
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

    arp_d = (arp_ipv4 *)arp_h->data;

    arp_d->dip = ntohs(arp_d->dip);
    arp_d->dmac = ntohs(arp_d->dmac);
    arp_d->sip = ntohs(arp_d->sip);
    arp_d->sip = ntohs(arp_d->sip);

    //更新 arp 表
    merge = update_arp_translation_table(arp_h, arp_d);

    // if(!(dev = netdev_get(arp_d->dip)))
    // {

    // }

    if(!merge && insert_arp_translation_table(arp_h, arp_d) != 0)
    {
        printf("ERR: No free space in ARP translation table\n");
        goto drop_skb;
    }

    switch (arp_h->opcode)
    {
        case ARP_REQUEST:   //0x0001
            arp_reply(skb, netdev);
            return;
        default:
            printf("ARP: Opcode not supported\n");
            goto drop_pkt;
    }
drop_skb:
    free(skb);
    return;
}