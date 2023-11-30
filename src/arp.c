#include "arp.h"
#include "netdev.h"

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

void arp_reply(netdev *dev, eth_hdr *hdr, arp_hdr *arp_h)
{
    arp_ipv4 *arp_d;
    int len;

    //设置 arp 响应
    arp_d = (arp_ipv4 *) arp_h->data;
    memcpy(arp_d->dmac, arp_d->smac, 6);
    arp_d->dip = arp_d->sip;
    memcpy(arp_d->smac, dev->hwaddr, 6);
    arp_d->sip = dev->addr;

    arp_h->opcode = ARP_REPLY;
    arp_h->opcode = htons(arp_h->opcode);
    arp_h->hwtype = htons(arp_h->hwtype);
    arp_h->protype= htons(arp_h->protype);

    len = sizeof(arp_hdr) + sizeof(arp_ipv4);
    netdev_transmit(dev, hdr, ETH_P_ARP, len, arp_d->dmac);
}