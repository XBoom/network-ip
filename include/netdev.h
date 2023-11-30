#ifndef NETDEV_H
#define NETDEV_H

typedef struct netdev
{
    uint32_t addr;           // IP地址
    unsigned char hwaddr[6]; // mac地址
} netdev;

void netdev_init(netdev *dev, char *addr, char *hwaddr);

void netdev_transmit(netdev *dev, eth_hdr *hdr,
                     uint16_t ethertype, int len, unsigned char *dst);

#endif