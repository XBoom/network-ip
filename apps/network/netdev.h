#ifndef __NETDEV_H__
#define __NETDEV_H__
#include "syshead.h"
#include "ethernet.h"

#define MAX_BUF_LEN (1600) // 缓存区节点大小
#define MAX_IP_LEN (32)  //  ip  地址长度
#define MAX_HW_LEN (6)     //  mac 地址长度
#define MAX_MAC_STR_LEN (18)    //mac stringg 地址长度

struct netdev
{
    uint32_t addr;     // IP地址
    uint8_t addr_len;  // IP地址长度
    uint8_t hwaddr[6]; // mac地址
    uint32_t mtu;      // 最小传输单位(数据链路层数据包大小)
};

struct sk_buff;

void netdev_init(struct netdev *dev, char *addr, char *hwaddr);

int netdev_transmit(struct sk_buff *skb, uint8_t *dst_hw, uint16_t ethertype);

void* netdev_rx_loop();

struct netdev *netdev_get(uint32_t sip);

void free_netdev();

#endif