#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include "netdev.h"
#include "route.h"
#include "list.h"
#include <pthread.h>

//sk_buff 数据节点
struct sk_buff
{
    struct list_head list; 
    rt_entry *rt;       //路由
    netdev *dev;        //虚拟网络设备
    int ref_cnt;         //用于引用计数
    uint16_t protocol;
    uint32_t len;
    uint32_t dlen;
    uint32_t seq;
    uint32_t end_seq;
    uint8_t *head;      //data 起始位置
    uint8_t *end;       //data 结束位置
    uint8_t *data;
    uint8_t *payload;
};

//sk_buff 头部节点
typedef struct sk_buff_head{
    struct list_head head;  // sk_buff 双向链表

    uint32_t qlen;  //链表长度
}sk_buff_head;

struct sk_buff *alloc_skb(unsigned int data_size);
void free_sk_buff(struct sk_buff *skb);
uint8_t *skb_push(struct sk_buff *skb, unsigned int len);
uint8_t *skb_head(struct sk_buff *skb);
void *skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_reset_header(struct sk_buff *skb);

static inline uint32_t sk_buff_queue_len(const sk_buff_head *list)
{
    return list->qlen;
}

static inline void sk_buff_queue_init(sk_buff_head *list)
{
    list_init(&list->head);
    list->qlen = 0;
}

static inline void sk_buff_queue_tail(sk_buff_head *list, struct sk_buff *new)
{
    list_add_tail(&new->list, &list->head);
    list->qlen += 1;
}

static inline struct sk_buff *sk_buff_dequeue(sk_buff_head *list)
{
    struct sk_buff *skb = list_first_entry(&list->head, struct sk_buff, list);
    list_del(&skb->list);
    list->qlen -= 1;

    return skb;
}

static inline int sk_buff_queue_empty(const sk_buff_head *list)
{
    return sk_buff_queue_len(list) < 1;
}

static inline struct sk_buff *sk_buff_peek(sk_buff_head *list)
{
    if (sk_buff_queue_empty(list)) return NULL;
        
    return list_first_entry(&list->head, struct sk_buff, list);
}

static inline void sk_buff_queue_free(sk_buff_head *list)
{
    struct sk_buff *skb = NULL;
    
    while ((skb = sk_buff_peek(list)) != NULL) {
        sk_buff_dequeue(list);
        skb->ref_cnt--;
        free_sk_buff(skb);
    }
}
#endif