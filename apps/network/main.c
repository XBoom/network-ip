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
    printf("recv frame type %04x \n", hdr->ethertype);
    switch(hdr->ethertype)
    {
        case ETH_P_ARP: //0x0806
            printf("found ARP\n");
            break;
        case ETH_P_IP:  //0x0800
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
        //这里是固定读取了 BUF_LEN 长度的报文
        struct sk_buff *skb = alloc_skb(BUF_LEN);    //分配一个 sk_buff,用来存储读到的数据
        if(tun_read((char *)skb->data, BUF_LEN) < 0)
        {
            print_error("read from tun_fd: %s\n", strerror(errno));
            free(skb);
            return NULL;
        }

        netdev_receive(skb);
        //print_hexdump(buf, BUF_LEN);
        //eth_hdr *hdr = init_eth_hdr(buf);

        //handle_frame(&dev, hdr);
    }
}