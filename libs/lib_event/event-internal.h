#ifndef __EVENT_INTERNAL_H__
#define __EVENT_INTERNAL_H__

#include "rb_tree.h"
#include "event.h"

struct event_base
{
    const struct eventop *evsel;
    void *evbase;
    int event_count;        /* counts number of total events */
    int event_count_active; /* counts number of active events */

    int event_gotterm; /* Set to terminate loop */

    /* active event management */
    struct event_list **activequeues;
    int nactivequeues;

    struct event_list eventqueue;
    struct timeval event_tv;

    RB_HEAD(event_tree, event)
    timetree;
};

#endif