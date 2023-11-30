#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"
#include "tuntap_if.h"
#include "basic.h"

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
void netdev_transmit(netdev *dev, eth_hdr *hdr, 
                     uint16_t ethertype, int len, unsigned char *dst)
{
    //主机字节序转为网络字节序 htons 将16位主机字节序转为网络字节序
    hdr->ethertype = htons(ethertype);

    memcpy(hdr->smac, dev->hwaddr, 6);   //设置源 mac 地址
    memcpy(hdr->dmac, dst, 6);           //地址目的 mac 地址

    len += sizeof(eth_hdr);

    tun_write((char *)hdr, len);
}