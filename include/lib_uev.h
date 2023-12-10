#ifndef __LIBUEV_H__
#define __LIBUEV_H__

#include <stdio.h>

// 事件上下文
struct uev_ctx_st
{
    uint8_t running;     // 运行状态
    int fd;              // epoll句柄
    uint32_t max_events; // epoll 最大时间数量
    struct uev_st *watchers;
    uint32_t workaround; /* For workarounds, e.g. redirected stdin */
};

// 使用宏定义用于隐藏私有数据成员
#define uev_private_st  \
    struct uev_st *next, *prev;    \
    int active; \
    int events; \
    void (*cb)(struct uev_st *, void *, int);  \
    void *args; \
    union   /* Watcher type */\
    { /* Cron watchers */   \
        struct  \
        {   \
            time_t when;    \
            int interval;   \
        } c;    \
        struct  /* Timer watchers, time in milliseconds */\
        {   \
            int timeout;    \
            int period; \
        } t;    \
    } u;    \
    uev_type_t type;

//事件
struct uev_st {
    uev_private_st //宏定义 private data for libuev internal engine

    //public data for users to reference
    int signo;  //configured signal
    int fd;     // active descriptor
    struct uev_ctx_st *ctx;    // watcher context

    // Extra data for certain watcher types  TODO 这是干嘛
    struct signalfd_siginfo siginfo;   //received signal
};


int uev_init(struct uev_ctx_st *ctx);
int uev_exit(struct uev_ctx_st *ctx);
int uev_run(struct uev_ctx_st *ctx, int flags);
int uev_timer_init(struct uev_ctx_st *ctx, struct uev_st *w, 
    uev_cb_t *cb, void *args, int timeout, int period);

#endif