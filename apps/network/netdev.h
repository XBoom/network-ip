#ifndef __NETDEV_H__
#define __NETDEV_H__
#include "syshead.h"
#include "ethernet.h"

struct netdev
{
    uint32_t addr;           // IP地址
    unsigned char hwaddr[6]; // mac地址
};

void netdev_init(struct netdev *dev, char *addr, char *hwaddr);

void netdev_transmit(struct sk_buff *skb, uint8_t *dst_hw, uint16_t ethertype);

#endif