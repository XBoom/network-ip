#include "common.h"
#include "syshead.h"
#include "route.h"
#include "dst.h"
#include "netdev.h"
#include "ip.h"

static LIST_HEAD(RoutingTable);   //路由表

//分配一条路由
static struct rt_entry *route_alloc(uint32_t dst, uint32_t gateway, uint32_t netmask,
                                   uint8_t flags, uint32_t metric, struct netdev *dev)
{
    struct rt_entry *rt = malloc(sizeof(struct rt_entry));
    list_init(&rt->list);

    rt->dst = dst;
    rt->gateway = gateway;
    rt->netmask = netmask;
    rt->flags = flags;
    rt->metric = metric;
    rt->ref = 0;
    rt->use = 0;
    rt->dev = dev;

    return rt;
}

//添加路由到路由表中
void route_add(uint32_t dst, uint32_t gateway, uint32_t netmask, uint8_t flags,
               uint32_t metric, struct netdev *dev)
{
    struct rt_entry *rt = route_alloc(dst, gateway, netmask, flags, metric, dev);

    list_add_tail(&rt->list, &routes);
}

struct rt_entry *route_find(uint32_t daddr)
{
    struct list_head *item;
    struct rt_entry *rt = NULL;

    list_for_each(item, &routes) {
        rt = list_entry(item, struct rt_entry, list);
        if ((daddr & rt->netmask) == (rt->dst & rt->netmask)) break;
        // If no matches, we default to to default gw (last item)
    }
    
    return rt;
}

//释放路由表
void free_routes()
{
    struct list_head *item, *tmp;
    struct rtentry *rt;
    
    list_for_each_safe(item, tmp, &routes) {
        rt = list_entry(item, struct rtentry, list);
        list_del(item);

        free(rt);
    }
}

//初始化路由
void route_init()
{
    route_add(loop->addr, 0, 0xff000000, RT_LOOPBACK, 0, loop);
    route_add(netdev->addr, 0, 0xffffff00, RT_HOST, 0, netdev);
    route_add(0, ip_parse(tapaddr), 0, RT_GATEWAY, 0, netdev);
}