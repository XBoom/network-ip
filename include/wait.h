#ifndef __WAIT_H__
#define __WAIT_H__

//锁机制

#include "syshead.h"
#include "common.h"

struct wait_lock {
    //条件变量，用于线程间的同步和通信
    pthread_cond_t ready;   
    //互斥锁，用于保护临界区，防止多个线程同时访问共享资源
    pthread_mutex_t lock;
    //线程的休眠状态
    uint8_t sleeping;
};

static inline int wait_init(struct wait_lock *w) {
    pthread_cond_init(&w->ready, NULL);
    pthread_mutex_init(&w->lock, NULL);
    w->sleeping = 0;
    
    return 0;
};

static inline int wait_wakeup(struct wait_lock *w) {
    pthread_mutex_lock(&w->lock);

    pthread_cond_signal(&w->ready);
    w->sleeping = 0;

    pthread_mutex_unlock(&w->lock);
    return 0;
};

static inline int wait_sleep(struct wait_lock *w) {
    w->sleeping = 1;
    pthread_cond_wait(&w->ready, &w->lock);
    
    return 0;
};

static inline void wait_free(struct wait_lock *w) {
    wait_wakeup(w);
    
    pthread_mutex_destroy(&w->lock);
    pthread_cond_destroy(&w->ready);
};


#endif