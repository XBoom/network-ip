#ifndef __PRIVATE_H__
#define __PRIVATE_H__

#include <stdio.h>
#include <sys.epoll.h>
#include <sys/signalfd.h>

// 遍历
#define _UEV_FOREACH(node, list)         \
    for (typeof(node) next, node = list; \
         node && (next = node->next, 1); \
         node = next;)

// 插入
#define _UEV_INSERT(node, list) \
    do                          \
    {       \
    }while (0) 
    

// 删除 TODO 

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
#define UEV_EVENT_MASK ()

// 事件上下文
typedef struct uev_ctx_st
{
    uint8_t running;     // 运行状态
    int fd;              // epoll句柄
    uint32_t max_events; // epoll 最大时间数量
    
} uev_ctx_st;

//使用宏定义用于隐藏私有数据成员
#define uev_private_st  \
    uev_st *next, *prev;    \
    int active; \
    int events; \

    void (*cb)(uev_st *, void *, int);  \
    void *args; \

    union       \
    {               \
        /* Cron watchers */ \
        struct  \
        {   \
            time_t when;    \
        }c; \

        /* Timer watchers, time in milliseconds */  \
        struct  \
        {   \
            int timeout;    \
            int period; \
        }t; \
    }u; \
    // Watcher type
    uev_type_t  
    
//TODO 下面函数啥意思
int _uev_watcher_init(uev_ctx_st *ctx, uev_st *w, uev_type_t type,
			void (*cb)(uev_st *, void *, int), void *data,
			int fd, int events);
int _uev_watcher_start(uev_st *w);
int _uev_watcher_stop(uev_st *w);
int _uev_watcher_active(uev_st *w);
int _uev_watcher_rearm(uev_st *w);

#endif