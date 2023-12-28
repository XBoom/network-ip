#include "network.h"

//网络初始化
uint32_t network_init()
{
    uint32_t ret = 0;
    //内存池 TODO

    //ARP 表初始化
    CHECK_RET(arp_init(), "arp table init failed");
}