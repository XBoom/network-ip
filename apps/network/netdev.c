#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"
#include "tuntap_if.h"
#include "basic.h"
#include "arp.h"

struct netdev *loop;
struct netdev *cur_netdev;
extern int running; //是否正在运行

//网络设备初始化
void netdev_init(struct netdev *dev, char *addr, char *hwaddr)
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
           
    cur_netdev = dev;
}

//分配一个网络设备
struct netdev *netdev_alloc(char *addr, char *hwaddr, uint32_t mtu)
{
    return NULL;
}

//虚拟设备传输
/**
 * dev 虚拟网络设备信息(ip、 mac)
 * hdr 二层数据帧
 * ethertype 以太网类型
 * len 负载藏都
 * dst 目的 mac
*/
int netdev_transmit(struct sk_buff * skb, uint8_t *dst_hw, uint16_t ethertype)
{
    //主机字节序转为网络字节序 htons 将16位主机字节序转为网络字节序
    struct netdev *dev;
    struct eth_hdr *hdr;

    dev = skb->dev;

    //由于数据是 ETH_HDR_LEN + ARP_HDR + ARP_IPV4
    //skb data指针指向 ARP_HDR
    skb_push(skb, ETH_HDR_LEN);

    hdr = (struct eth_hdr *)skb->data;

    memcpy(hdr->dmac, dst_hw, dev->addr_len);
    memcpy(hdr->smac, dev->hwaddr, dev->addr_len);

    hdr->ethertype = htons(ethertype);
    show_eth_hdr(hdr);

    return tun_write((char *)skb->data, skb->len);;
}

//接受虚拟设备报文
static void netdev_receive(struct sk_buff *skb)
{
    struct eth_hdr *hdr = get_eth_hdr(skb);   
    show_eth_hdr(hdr);
    switch(hdr->ethertype)
    {
        case ETH_P_ARP: //0x0806
            arp_receive(skb);
            break;
        case ETH_P_IP:  //0x0800
            printf("found ipv4");
            free_skb(skb);
            break;
        case ETH_P_IPV6:    //TODO 
            printf("found ipv6");
            free_skb(skb);
            break;
        default:
            printf("unrecognized ethertype %04x \n", hdr->ethertype);
            free_skb(skb);
    } 
}

//继续读取网络包
void* netdev_rx_loop()
{
    while(running)
    {
        struct sk_buff *skb = alloc_skb(MAX_BUF_LEN);

        if(tun_read((char *)skb->data, MAX_BUF_LEN) < 0)
        {
            printf("ERR: Read from tun_fd");
            free_skb(skb);
            return NULL;
        }

        netdev_receive(skb);
    }
    return NULL;
}

//根据 包的来源 ip 获取虚拟网络设备
struct netdev* netdev_get(uint32_t sip)
{
    if(!cur_netdev) return NULL;

    if(cur_netdev->addr == sip) 
        return cur_netdev;
    else 
        return NULL;
}

void free_netdev()
{
    if(loop)
        free(loop);
    if(cur_netdev)
        free(cur_netdev);
}
