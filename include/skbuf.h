#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include "netdev.h"
#include "route.h"
#include "list.h"
#include <pthread.h>

typedef struct sk_buff
{
    struct list_head list; 
    rt_entry *rt;    //路由
    netdev *dev;    //虚拟网络设备
    int refcnt; 
    uint16_t protocol;
    uint32_t len;
    uint32_t dlen;
    uint32_t seq;
    uint32_t end_seq;
    uint8_t *end;
    uint8_t *head;
    uint8_t *data;
    uint8_t *payload;
}sk_buff;

typedef struct sk_buff_head{
    struct list_head head;

    uint32_t qlen;
}sk_buff_head;

#endif