#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

//双向链表
struct list_head 
{
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD(name) \
    struct list_head name = {&(name), &(name)}

//双向链表初始化
static inline void list_init(struct list_head *head)
{
    head->prev = head->next = head;
}

//链表头添加节点 head->add->....
static inline void list_add(struct list_head *elem, struct list_head *head)
{
    head->next->prev = elem;
    elem->next = head->next;
    elem->prev = head;
    head->next = elem;
}

//链表头添加节点 add->head->....
static inline void list_add_tail(struct list_head *elem, struct list_head *head)
{
    head->prev->next = elem;
    elem->prev = head->prev;
    elem->next = head;
    head->prev = elem;
}

//删除节点
static inline void list_del(struct list_head *elem)
{
    //elem 只有一个节点或者为空？
    struct list_head *prev = elem->prev;
    struct list_head *next = elem->next;
    prev->next = next;
    next->prev = prev;
}

//获取元素
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

//获取第一个元素
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

//遍历每一个元素
#define list_for_each(pos, head) \
    for(pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, p, head) \
    for(pos = (head)->next, p = pos->next; \
        pos != (head);                      \
        pos = p, p = pos->next)

//判断是否为空
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

#endif