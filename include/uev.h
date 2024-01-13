#ifndef __UEV_H__
#define __UEV_H__

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>

//TODO 测试一下这里 (next = node->next, 1) 的作用
#define _UEV_FOREACH(node, list)    \
    for(typeof(node) next, node = list; \
         node && (next = node->next, 1); \
         node = next)

// 将 node 插入到 list 前面
#define _UEV_INSERT(node, list) do {    \
    typeof(node) next;    \
    next = list;    \
    list = node;    \
    if(next)    \
        next->prev = node;  \
    node->next = next;  \
    node->prev = NULL;  \
}while(0)

// 从 list 中删除 node
#define _UEV_REMOVE(node ,list) do{ \
    typeof(node) prev, next;   \
    prev = node->prev;  \
    next = node->next;  \
    if(prev)    \
        prev->next = next;  \
    if(next)    \
        next->prev = prev;  \
    node->prev = NULL;  \
    node->next = NULL;  \
    if(list == node)    \
        list = next;    \
}while(0)

/*
Cron:
定时任务调度器： Cron是一种用于在预定时间执行任务的任务调度器。它在Unix和类Unix系统中广泛使用。
时间规则： 用户可以通过Cron表达式指定任务的执行时间。Cron表达式包含由空格分隔的时间和日期字段，用于指定任务的分钟、小时、日期等。
周期性任务： Cron允许设置周期性任务，例如每天的特定时间执行、每周执行一次等。
例如，Cron表达式 0 2 * * * 表示每天凌晨2点执行任务。

Timer:
编程中的定时器： Timer是编程语言或框架中的一个概念，通常是一个计时器对象，用于在指定的时间后触发某个操作或任务。
精确性： Timers通常更精确，可以实现毫秒级别的定时。
应用级任务： Timers通常用于应用程序级别的任务调度，例如在一段时间后执行某个函数或处理某个事件
*/
// 事件类型
typedef enum
{
    UEV_IO_TYPE = 1, // io
    UEV_SIGNAL_TYPE, // signal
    UEV_TIMER_TYPE,  // timer
    UEV_CRON_TYPE,   // cron
    UEV_EVENT_TYPE,  // event
} uev_type_t;

// 事件掩码
#define UEV_EVENT_MASK (UEV_ERROR | UEV_READ | UEV_WRITE | UEV_PRI | \
            UEV_RDHUP | UEV_HUP | UEV_EDGE | UEV_ONESHOT)

//struct uev_st;  //向前声明，告之这里有一个结构体 uev_st

// 事件上下文
struct uev_ctx_st
{
    uint8_t running;     // 运行状态
    int fd;              // epoll 句柄
    uint32_t max_events; // epoll 最大时间数量
    struct uev_st *watchers;    //观察的上下文
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

#define MAX_EV_EVNETS (10)  //TODO 默认值，文件读取

//TODO 下面的状态分别代表着什么
#define UEV_NONE        0   //normal loop
#define UEV_ERROR   EPOLLERR    //error flag
#define UEV_READ    EPOLLIN     //poll for reading
#define UEV_WRITE   EPOLLOUT    //poll for writing
#define UEV_PRI     EPOLLPRI    //priority message
#define UEV_HUP     EPOLLHUP    //hungup event
#define UEV_RDHUP   EPOLLRDHUP  //peer shutdown
#define UEV_EDGE    EPOLLET     //edge triiggered   //为什么没有垂直触发
#define UEV_ONESHOT EPOLLONESHOT    //one-shot event //这个是垂直触发吗

//执行标志
#define UEV_ONCE    1       // run loop once
#define UEV_NONBLOCK    2   // exit if no event

//检查 io watcher is active or stopped
#define uev_io_active(w) _uev_watcher_acitve(w)
/** Check if signal watcher is active or stopped */
#define uev_signal_active(w) _uev_watcher_active(w)
/** Check if timer is active or stopped */
#define uev_timer_active(w)  _uev_watcher_active(w)
/** Check if cron timer watcher is active or stopped */
#define uev_cron_active(w)   _uev_watcher_active(w)
/** Check if event watcher is active or stopped */
#define uev_event_active(w)  _uev_watcher_active(w)

//事件回调
typedef void (uev_cb_t)(struct uev_st *w, void *args, int events);

//事件处理上下文
int uev_init(struct uev_ctx_st *ctx);
int uev_init1(struct uev_ctx_st *ctx, int max_events); //epoll使用(不过高版本也没用)
int uev_exit(struct uev_ctx_st *ctx);
int uev_run(struct uev_ctx_st *ctx, int flag);

//IO事件
int uev_io_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args, int fd, int events);
int uev_io_set(struct uev_st *w, int fd, int events);
int uev_io_start(struct uev_st *w);
int uev_io_stop(struct uev_st *w);

//定时器
int uev_timer_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args, int timeout, int period);
int uev_timer_set(struct uev_st *w, int timeout, int period);
int uev_timer_start(struct uev_st *w);
int uev_timer_stop(struct uev_st *w);

// TODO cron 与 timer 的区别
int uev_cron_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args, time_t when, time_t interval);
int uev_cron_set(struct uev_st *w, time_t when, time_t interval);
int uev_cron_start(struct uev_st *w);
int uev_cron_stop(struct uev_st *w);

int uev_signal_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args, int signo);
int uev_signal_set(struct uev_st *w, int signo);
int uev_signal_start(struct uev_st *w);
int uev_signal_stop(struct uev_st *w);

int uev_event_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args);
int uev_event_post(struct uev_st *w);
int uev_event_stop(struct uev_st *w);

int _uev_watcher_init(struct uev_ctx_st *ctx, struct uev_st *w, 
    uev_type_t type, uev_cb_t *cb, void * data, int fd, int events);
int _uev_watcher_active(struct uev_st *w);
int _uev_watcher_start(struct uev_st *w);
int _uev_watcher_stop(struct uev_st *w);
int _uev_watcher_rearm(struct uev_st *w);

#endif