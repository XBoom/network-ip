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
#define UEV_ONCE    1       //执行一次

#endif