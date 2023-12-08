#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include "netdev.h"
#include "route.h"
#include "list.h"
#include <pthread.h>

//网络缓冲区 用于表示收到或要发送的数据包

//sk_buff 数据节点
typedef struct sk_buff_st
{
    struct list_head list;  //用于将缓冲区链接到链表中，通常用于管理多个缓冲区
    rt_entry *rt;           //指向路由表条目的指针，用于指示数据包的路由信息
    netdev *dev;            //指向虚拟网络设备的指针，表示数据包通过哪个网络设备进行传输
    int ref_cnt;            //用于引用计数，用来跟踪缓冲区的引用次数。在多处需要访问同一缓冲区时，通过引用计数来确保正确释放内存
    uint16_t protocol;      //表示数据包使用的网络协议类型
    uint32_t len;           //整个缓冲区的长度
    uint32_t dlen;          //数据的长度
    uint32_t seq;           //与 end_seq 用于表示数据包的序列号范围
    uint32_t end_seq;
    uint8_t *head;          //data 起始位置
    uint8_t *end;           //data 结束位置
    uint8_t *data;
    uint8_t *payload;       //指向数据包负载的指针
}sk_buff_st;

//sk_buff 头部节点
typedef struct sk_buff_head{
    struct list_head head;  // sk_buff 双向链表

    uint32_t qlen;  //链表长度
}sk_buff_head;

sk_buff_st *alloc_skb(unsigned int size);
void free_skb(sk_buff_st *skb);
uint8_t *skb_push(sk_buff_st *skb, unsigned int len);
uint8_t *skb_head(sk_buff_st *skb);
void *skb_reserve(sk_buff_st *skb, unsigned int len);
void skb_reset_header(sk_buff_st *skb);

//初始化缓冲区队列
static inline void sk_buff_queue_init(sk_buff_head *list)
{
    list_init(&list->head);
    list->qlen = 0;
}

//缓冲区队列中的元素数量
static inline uint32_t sk_buff_queue_len(const sk_buff_head *list)
{
    return list->qlen;
}

//数据包的入队
static inline void sk_buff_queue_tail(sk_buff_head *list, struct sk_buff *new)
{
    list_add_tail(&new->list, &list->head);
    list->qlen += 1;
}

//数据包的出对
static inline sk_buff_st *sk_buff_dequeue(sk_buff_head *list)
{
    sk_buff_st *skb = list_first_entry(&list->head, sk_buff_st, list);
    list_del(&skb->list);
    list->qlen -= 1;

    return skb;
}

//判断缓冲区是否为空
static inline int sk_buff_queue_empty(const sk_buff_head *list)
{
    return sk_buff_queue_len(list) < 1;
}

//查看缓冲区头部的数据包(不从链表移除)
static inline sk_buff_st *sk_buff_peek(sk_buff_head *list)
{
    if (skb_queue_empty(list)) return NULL;
        
    return list_first_entry(&list->head, sk_buff_st, list);
}

static inline void sk_buff_queue_free(sk_buff_head *list)
{
    sk_buff_st *skb = NULL;
    
    while ((skb = sk_buff_peek(list)) != NULL) {
        sk_buff_dequeue(list);
        skb->ref_cnt--;
        free_sk_buff(skb);
    }
}
#endif