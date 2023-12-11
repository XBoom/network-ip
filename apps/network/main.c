#include "syshead.h"
#include "basic.h"
#include "utils.h"
#include "tuntap_if.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"

#define BUF_LEN 100

int main(int argc, char** argv)
{
    char buf[BUF_LEN];
    struct netdev dev;

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
    }
}