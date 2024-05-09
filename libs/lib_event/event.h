#ifndef __EVENT_H__
#define __EVENT_H__

#include "rb_tree.h"
#include "queue.h"
#include "times.h"

// 事件链表状态 TODO
#define EVLIST_TIMEOUT 0x01
#define EVLIST_INSERTED 0x02
#define EVLIST_SIGNAL 0x04
#define EVLIST_ACTIVE 0x08
#define EVLIST_INTERNAL 0x10
#define EVLIST_INIT 0x80

/* EVLIST_X_ Private space: 0x1000-0xf000 */
#define EVLIST_ALL (0xf000 | 0x9f)

#define EV_TIMEOUT 0x01
#define EV_READ 0x02
#define EV_WRITE 0x04
#define EV_SIGNAL 0x08
#define EV_PERSIST 0x10 /* Persistant event */

#define TAILQ_ENTRY(type)                                              \
    struct                                                             \
    {                                                                  \
        struct type *tqe_next;  /* next element */                     \
        struct type **tqe_prev; /* address of previous next element */ \
    }

// 事件
struct event
{
    TAILQ_ENTRY(event)
    ev_next; // 事件链表
    TAILQ_ENTRY(event)
    ev_active_next; // 激活链表
    TAILQ_ENTRY(event)
    ev_signal_next; // 信号链表
    RB_ENTRY(event)
    ev_timeout_node; // 超时链表

    struct event_base *ev_base; // 事件基础结构
    int ev_fd;
    short ev_events;
    short ev_ncalls;
    short *ev_pncalls; /* Allows deletes in callback */

    struct timeval ev_timeout;

    int ev_pri; // 事件优先级(越小优先级越高)

    void (*ev_callback)(int, short, void *arg);
    void *ev_arg;

    int ev_res; /* result passed to event callback */
    int ev_flags;
};

// 事件句柄
#define EVENT_FD(ev) (int)ev->ev_fd

// 事件双向链表
TAILQ_HEAD(event_list, event);

struct eventop
{
    char *name;
    void *(*init)(void);
    int (*add)(void *, struct event *);
    int (*del)(void *, struct event *);
    int (*recalc)(struct event_base *, void *, int);
    int (*dispatch)(struct event_base *, void *, struct timeval *);
};

#define TIMEOUT_DEFAULT \
    {                   \
        5, 0            \
    }

void *event_init(void);   // 事件初始化
int event_dispatch(void); // 事件分发
int event_base_dispatch(struct event_base *);

typedef void (*event_log_cb)(int severity, const char *msg);
void event_set_log_callback(event_log_cb cb);

/* Associate a different event base with an event */
int event_base_set(struct event_base *, struct event *);

#define EVLOOP_ONCE 0x01
#define EVLOOP_NONBLOCK 0x02
int event_loop(int);
int event_base_loop(struct event_base *, int);
int event_loopexit(struct timeval *); /* Causes the loop to exit */
int event_base_loopexit(struct event_base *, struct timeval *);

#define evtimer_add(ev, tv) event_add(ev, tv)
#define evtimer_set(ev, cb, arg) event_set(ev, -1, 0, cb, arg)
#define evtimer_del(ev) event_del(ev)
#define evtimer_pending(ev, tv) event_pending(ev, EV_TIMEOUT, tv)
#define evtimer_initialized(ev) ((ev)->ev_flags & EVLIST_INIT)

#define timeout_add(ev, tv) event_add(ev, tv)
#define timeout_set(ev, cb, arg) event_set(ev, -1, 0, cb, arg)
#define timeout_del(ev) event_del(ev)
#define timeout_pending(ev, tv) event_pending(ev, EV_TIMEOUT, tv)
#define timeout_initialized(ev) ((ev)->ev_flags & EVLIST_INIT)

#endif