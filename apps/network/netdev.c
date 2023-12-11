#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"
#include "tuntap_if.h"
#include "basic.h"

netdev *loop;
netdev *cur_netdev;

//网络设备初始化
void netdev_init(netdev *dev, char *addr, char *hwaddr)
{
    CLEAR(*dev);

    /*
        #include <arpa/inet.h>
        int inet_pton(int af, const char *src, void *dst);
        brief:  将src指向的字符串解析为指定地址族的二进制形式；
        return: 成功返回1，失败返回0.并设置errno来指示错误类型
        param:
            af 指定协议簇(address family), 常见的 AF_INET(IPv4)和 AF_INET6(IPv6)
            src 字符串指针，表示要转换的ip地址
            dst 指向存储结果的缓存区指针
    */

    if (inet_pton(AF_INET, addr, &dev->addr) != 1)
    {
        perror("parsing inet address failed \n");
        exit(1);
    }

    //将字符串hwaddr 中的十六进制解析并存储到dev->hwaddr数组中
    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0],
           &dev->hwaddr[1],
           &dev->hwaddr[2],
           &dev->hwaddr[3],
           &dev->hwaddr[4],
           &dev->hwaddr[5]);
}

//虚拟设备传输
/**
 * dev 虚拟网络设备信息(ip、 mac)
 * hdr 二层数据帧
 * ethertype 以太网类型
 * len 负载藏都
 * dst 目的 mac
*/
void netdev_transmit(struct sk_buff *skb, uint8_t *dst_hw, uint16_t ethertype)
{
    //主机字节序转为网络字节序 htons 将16位主机字节序转为网络字节序
   struct netdev *dev;
    struct eth_hdr *hdr;
    int ret = 0;

    dev = skb->dev;

    skb_push(skb, ETH_HDR_LEN);

    hdr = (struct eth_hdr *)skb->data;

    memcpy(hdr->dmac, dst_hw, dev->addr_len);
    memcpy(hdr->smac, dev->hwaddr, dev->addr_len);

    hdr->ethertype = htons(ethertype);
    eth_dbg("out", hdr);

    ret = tun_write((char *)skb->data, skb->len);

    return ret;
}

//接受虚拟设备报文
static int netdev_receive(struct sk_buff *skb)
{
    struct eth_hdr *hdr = get_eth_hdr(skb);   
    show_eth_hdr(hdr);
    switch(hdr->ethertype)
    {
        case ETH_P_ARP: //0x0806
            printf("found ARP \n");
            arp_receive(skb);
            break;
        case ETH_P_IP:  //0x0800
            printf("found ipv4");
            break;
        case ETH_P_IPV6:    //TODO 
        default
            printf("unrecognized ethertype %04x \n", hdr->ethertype);
            free_skb(skb);
    } 
}

//根据 IP 地址获取网络设备
netdev *netdev_get(uint32_t sip)
{
    if(cur_netdev->addr == sip)
    {
        return cur_netdev; 
    }
    return NULL;
}

