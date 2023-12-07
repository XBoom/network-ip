#ifndef __PRIVATE_H__
#define __PRIVATE_H__

#include <stdio.h>
#include <sys.epoll.h>
#include <sys/signalfd.h>

//遍历
#define _UEV_FOREACH(node, list)    \
    for(typeof(node)next, node = list;  \
        node && (next = node->next, 1); \
        node = next;)

//插入
#define _UEV_INSERT(node, list) do {    \


}

//删除

//事件类型
typedef enum {
    UEB_IO_TYPE = 1,    //io
    UEV_SIGNAL_TYPE,    //signal
    UEV_TIMER_TYPE,     //timer
    UEV_CRON_TYPE,      //cron
    UEV_EVENT_TYPE,     //event
}uev_type_t;

//事件掩码
#define UEV_EVENT_MASK ()

//事件上下文
typedef struct uev_ctx_st {
    uint8_t running;    //运行状态
    int fd;         // epoll句柄
    uint32_t max_events;  // epoll 最大时间数量
}uev_ctx_st;







#endif