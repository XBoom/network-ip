#include "syshead.h"
#include "skbuff.h"
#include "list.h"

//分配一个帧缓存区
struct sk_buff *alloc_skb(unsigned int data_size)
{
    struct sk_buff *skb = malloc(sizeof(struct sk_buff));
    memset(skb, 0, sizeof(struct sk_buff));

    //分配缓存区大小
    skb->data = malloc(data_size);
    memset(skb->data, 0, data_size);

    skb->ref_cnt = 0;                   //引用数量为0
    skb->head = skb->data;              //数据起始指针
    skb->end = skb->data + data_size;   //数据结束指针

    list_init(&skb->list);
    return skb;
}

//释放struct sk_buff对象
void free_skb(struct sk_buff *skb)
{
    if(NULL == skb) return;

    if(skb->ref_cnt < 1)
    {
        free(skb->head);    //data
        skb->head = NULL;
        free(skb);
    }
}

void *skb_reserve(struct sk_buff *skb, unsigned int len)
{
    if(NULL == skb || len == 0) return skb;

    skb->data += len;
    return skb->data;
}

uint8_t *skb_push(struct sk_buff *skb, unsigned int len)
{
    if(NULL == skb || len == 0) return skb->data;

    skb->data -= len;
    skb->len += len;

    return skb->data;
}

uint8_t *skb_head(struct sk_buff *skb)
{
    if(NULL == skb) return NULL;

    return skb->head;
}

void skb_reset_header(struct sk_buff *skb)
{
    skb->data = skb->end - skb->dlen;
    skb->len = skb->dlen;
}