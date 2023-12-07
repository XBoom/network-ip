/*
 * @Author: Yuan Kang 728081687@qq.com
 * @Date: 2023-12-07 22:17:09
 * @LastEditors: Yuan Kang 728081687@qq.com
 * @LastEditTime: 2023-12-07 23:09:01
 * @FilePath: \network-ip\libs\lib_euv\src\uev.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __UEV_H__
#define __UEV_H__

#include "private.h"   //为什么可以循环引用

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

//事件
typedef struct uev_st {
    uev_private_t type; //private data for libuev internal engine

    //public data for users to reference
    int signo;  //configured signal
    int fd;     // active descriptor
    uev_ctx_st *ctx;    // watcher context

    // Extra data for certain watcher types  TODO 这是干嘛
    struct signalfd_siginfo siginfo;   //received signal
}uev_st;

#endif