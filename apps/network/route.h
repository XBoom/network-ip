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
struct rt_entry
{
    struct list_head list;
    uint32_t dst;           //目的地IP地址，表示数据包要去的地方
    uint32_t gateway;       //网关IP地址，指定了数据包离开本地网络时发送到下一个路由地址
    uint32_t netmask;       //子网掩码，用于确定IP地址的网络位与本机位
    uint8_t flags;          //路由条目的标志位，记录路由状态(UG表示为默认网关)
    uint32_t metric;        //路由的度量值，优先级(越小优先级越高)
    uint32_t ref;           //引用次数，有多少条路由表当前在引用这个路由项
    uint32_t use;           //使用计数，显示自从系统启动以来有多少个数据包使用了这个路由
    struct netdev *dev;     //接口，指定数据库包哪个网络接口出去
};

//TODO 多个路由表

void route_init();  //路由初始化
struct rt_entry *route_lookup(uint32_t d_addr);    //查询路由地址
void free_routes(); //释放路由

#endif