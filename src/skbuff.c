#include "syshead.h"
#include "skbuff.h"
#include "list.h"

sk_buff_st *allock_skb(unsigned int data_size)
{
    sk_buff_st *skb = MALLOC(sizeof(sk_buff_st));
    memset(skb, 0, sizeof(sk_buff_st));

    skb->data = MALLOC(sk_buff_st);
    memset(skb->data, 0, data_size);

    skb->ref_cnt = 0;
    skb->head = skb->data;
    skb->end = skb->data + data_size;

    list_init(&skb->list);
    return skb;
}

//释放sk_buff_st对象
void free_sk_buff(sk_buff_st *skb)
{
    if(NULL == skb) return;

    if(skb->ref_cnt < 1)
    {
        FREE(skb->head);    //data
        skb->head = NULL;
        FREE(skb);
    }
}

void *sk_buff_reserve(sk_buff_st *skb, unsigned int len)
{
    if(NULL == skb || len == 0) return skb;

    skb->data += len;
    return skb->data;
}

uint8_t *sk_buff_push(sk_buff_st *skb, unsigned int len)
{
    if(NULL == skb || len == 0) return skb;

    skb->data -= len;
    skb->len += len;

    return skb->data;
}

uint8_t *sk_buff_head(sk_buff_st *skb)
{
    if(NULL == skb) return skb;

    return skb->head;
}

void sk_buff_header(sk_buff_st *skb)
{
    skb->data = skb->end - skb->dlen;
    skb->len = skb->dlen;
}