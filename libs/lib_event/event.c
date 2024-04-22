#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "event.h"
#include "event-internal.h"

#include "lib_log.h"
#include "lib_mem.h"
#include "utils.h"
#include "queue.h"

const struct eventop *eventops[] = {
#ifdef HAVE_WORKING_KQUEUE
    &kqops,
#endif
#ifdef HAVE_EPOLL
    &epollops,
#endif
#ifdef HAVE_DEVPOLL
    &devpollops,
#endif
#ifdef HAVE_RTSIG
    &rtsigops,
#endif
#ifdef HAVE_POLL
    &pollops,
#endif
#ifdef HAVE_SELECT
    &selectops,
#endif
#ifdef WIN32
    &win32ops,
#endif
    NULL};

struct event_list signalqueue;
struct event_base *g_event_base = NULL; // 事件初始化

/* Handle signals - This is a deprecated interface */
int (*event_sigcb)(void);  /* Signal callback when gotsig is set */
volatile int event_gotsig; /* Set in signal handler */

// 事件初始化
void *event_init(void)
{
    int i;
    g_event_base = X_MALLOC(sizeof(struct event_base));
    CHECK_RET_GOTO(g_event_base == NULL,
                   end, "malloc event base failed");

    event_sigcb = NULL;
    event_gotsig = 0;
    gettimeofday(&g_event_base->event_tv, NULL); // TODO 获取当前时间

    RB_INIT(&g_event_base->timetree);      // 红黑树事件记录(为什么不使用堆 TODO)
    TAILQ_INIT(&g_event_base->eventqueue); // 事件双向链表
    TAILQ_INIT(&signalqueue);              // 信号链表

    for (i = 0; eventops[i] && !g_event_base->evbase; i++)
    {
        g_event_base->evsel = eventops[i];
        g_event_base->evbase = g_event_base->evsel->init(); // 事件初始化
    }

end:
    return g_event_base;
}