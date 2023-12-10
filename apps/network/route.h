#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "list.h"
#include "netdev.h"

#define RT_LOOPBACK 0x01
#define RT_GATEWAY 0x02
#define RT_HOST 0x04
#define RT_REJECT 0x08
#define RT_UP 0x10

//路由对象
typedef struct rt_entry
{
    struct list_head list;
    uint32_t dst;
    uint32_t gateway;
    uint32_t netmask;
    uint8_t flags;
    uint32_t metric;
    netdev *dev;        //网络虚拟设备
}rt_entry;

void route_init();  //路由初始化
rt_entry *route_lookup(uint32_t d_addr);    //查询路由地址
void free_routes(); //释放路由

#endif