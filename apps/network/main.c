#include "syshead.h"
#include "basic.h"
#include "utils.h"
#include "tuntap_if.h"
#include "arp.h"
#include "netdev.h"

int running = 1;

int main(int argc, char** argv)
{
    struct netdev dev;

    //1. 分配一个虚拟接口
    tun_init(); 

    //2. 设置虚拟网络设备地址
    netdev_init(&dev, "10.10.0.4", "00:0c:29:6d:50:25");    //解析地址信息到 dev 结构体中

    //网络初始化
    network_init();

    //4. 开始接受报文
    netdev_rx_loop();
out:
    LOG_ERROR("program exit");
    return 0;
}