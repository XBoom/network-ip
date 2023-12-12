#include "syshead.h"
#include "basic.h"
#include "utils.h"
#include "tuntap_if.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"

#define BUF_LEN 100
int running = 1;

int main(int argc, char** argv)
{
    char buf[BUF_LEN];
    struct netdev dev;
    tun_init(); //分配一个虚拟接口

    netdev_init(&dev, "10.10.0.4", "00:0c:29:6d:50:25");    //解析地址信息到 dev 结构体中

    arp_init(); //初始化 arp 逻辑

    netdev_rx_loop();
}