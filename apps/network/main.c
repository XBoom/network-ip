#include "syshead.h"
#include "basic.h"
#include "utils.h"
#include "tuntap_if.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"

#define BUF_LEN 100

//处理二层帧 
void handle_frame(netdev *netdev, eth_hdr* hdr)
{
    printf("recv frame type %x", hdr->ethertype);
    switch(hdr->ethertype)
    {
        case ETH_P_ARP:
            printf("found ARP\n");
            break;
        case ETH_P_IP:
            printf("found ipv4\n");
            break;
        default:
            printf("unrecognized ethertype %x \n", hdr->ethertype);
    }
}

int main(int argc, char** argv)
{
    char buf[BUF_LEN];
    netdev dev;

    CLEAR(buf);

    tun_init(); //分配一个虚拟接口

    netdev_init(&dev, "10.10.0.4", "00:0c:29:6d:50:25");    //解析地址信息到 dev 结构体中

    arp_init(); //初始化 arp 逻辑

    while(1) 
    {
        if(tun_read(buf, BUF_LEN) < 0)
        {
            print_error("read from tun_fd: %s\n", strerror(errno));
        }

        //print_hexdump(buf, BUF_LEN);
        eth_hdr *hdr = init_eth_hdr(buf);

        handle_frame(&dev, hdr);
    }
}