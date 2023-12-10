#include <errno.h>
#include <fcntl.h>		/* O_CLOEXEC */
#include <string.h>		/* memset() */
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/select.h>		/* for select() workaround */
#include <sys/signalfd.h>	/* struct signalfd_siginfo */
#include <unistd.h>		/* close(), read() */

#include "uev.h"

/**
 * @brief 事件上下文初始化(构建 epoll 实例)
 * @param
 *  uev_ctx_st *ctx 事件上下文
 *  int close_old 是否关闭旧的
 * @return 
 *  0 成功; -1 异常
*/
static int _init(struct uev_ctx_st *ctx, int close_old)
{
    int fd;
    /**
     * 创建一个新的 epoll 实例
     *  EPOLL_CLOEXEC 表示在 exec 被调用时关闭文件描述符，
     *      这是一种安全的做法，以防止子进程继承不必要的文件描述符
     *  EPOLL_NONBLOCK 设置文件描述符为非阻塞模式。
     *      在非阻塞模式下，当执行 I/O 操作时，如果数据不可用或者无法立即进行操作，系统不会让进程陷入阻塞状态，
     *      而是会立即返回一个错误或者一个特殊的值，告诉进程当前无法进行操作
    */
    fd = epoll_create1(EPOLL_CLOEXEC);  //TODO 这里设置 O_NONBLOCK 之后定时器失效
    if(fd < 0)
        return -1;  //TODO 添加错误日志

    if(close_old)   //是否关闭旧的
        close(ctx->fd);  //TODO 如果不关闭，旧的怎么办
    
    ctx->fd = fd;
    return 0;   
}

//判单是否有事件
static int has_data(int fd)
{
    struct timeval timeout = {0, 0};    //select 的超时时间

    fd_set fds; //句柄合集
    int n = 0;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /**
     *  int select(int n, fd_set* readfds, fd_set* writefds, fd_set* errorfds, struct timeval* timeout);
     *  n 是指集合中所有文件木舒服的最大值加1 ！！！！
     *  readfds 表示监视这些文件描述符的读变化，如果有一个可读，那么select就会返回一个大于0的值
     *  writefds  表示监视这些文件描述符的写变化，如果有一个可读，那么select就会返回一个大于0的值
     *  errorfds  表示监视文件错误异常
     *  timeout 根据timeout 判断是否超时，如果超时，则返回0；如果发生错误，则返回负数。
     *      如果传入 NULL，则处于阻塞状态，一直等到监视的文件描述符有变化为止
     *      如果传入 0秒0毫秒，则处于非阻塞状态，但是不管文件描述符是否变化，都会立即返回
     *      如果传入 大于0，就是等待的超时时间，在timeout内阻塞
     * 
     *  注意：
     *      readfds 和 timeout 在每次执行 select 前都要重新初始化
     *          readfds 不清空检测不到描述符变化
     *          timeout 每次都要初始化其值，否则会被默认初始化为0
    */
    // FIONREAD 用于获取套接字接收缓冲区中的未读数据大小
    if(select(1, &fds, NULL, NULL, &timeout) > 0)
        return ioctl(0, FIONREAD, &n) == 0 && n > 0;

    return 0;
}

//事件观察者初始化
int _uev_watcher_init(struct uev_ctx_st *ctx, struct uev_st *w, 
    uev_type_t type, uev_cb_t *cb, void * data, int fd, int events)
{
    if(NULL == ctx || NULL == w) 
    {
        errno = EINVAL;
        return -1;
    }

    w->ctx    = ctx;
	w->type   = type;
	w->active = 0;      // TODO 使用枚举类型代替
	w->fd     = fd;
	w->cb     = cb;
	w->args    = data;
	w->events = events;

    return 0;
}

int _uev_watcher_start(struct uev_st *w)
{   
    struct epoll_event ev;  //TODO 具体实现

    if(!w || w->fd < 0 || !w->ctx)
    {
        errno = EINVAL;
        return -1;
    }

    if (_uev_watcher_active(w)) //如果已经开始，则直接返回
		return 0;
    
    //EPOLLRDHUP 如果文件描述符对应的连接被对方关闭、或者对方关闭了写操作的一端，就会触发这个事件
    ev.events = w->events | EPOLLRDHUP; 
    ev.data.ptr = w;

    //添加事件
    // TODO w->ctx->fd 与 w->fd 的区别
    if(epoll_ctl(w->ctx->fd, EPOLL_CTL_ADD, w->fd, &ev) < 0)
    {
        if(errno != EPERM) return -1;

        //TODO 为什么这两个要退出
        if (w->type != UEV_IO_TYPE || w->events != UEV_READ)
			return -1;

        //STDIN_FILENO 通常定义在头文件 unistd.h 中。它表示标准输入流的文件描述符
        //UNIX 系统中，0、1、2 分别对应标准输入、标准输出和标准错误输出
        // TODO 为什么这里不是输入就退出？
        if(w->fd != STDIN_FILENO)
            return -1;
        
        w->ctx->workaround = 1;
        w->active = -1; 
    } else {
        w->active = 1;
    }

    _UEV_INSERT(w, w->ctx->watchers);   //w 添加到上下文的 watchers

    return 0;
}

/**
 * 停止事件 
 * 更改事件状态，从 watcher 中删除，从 epoll 中删除
 * */
int _uev_watcher_stop(struct uev_st *w)
{
    if(!w)
    {
        errno = EINVAL;
        return -1;
    }

    if(!_uev_watcher_active(w))
        return 0;
    
    w->active = 0;

    _UEV_REMOVE(w, w->ctx->watchers);

    if(epoll_ctl(w->ctx->fd, EPOLL_CTL_DEL, w->fd, NULL) < 0)
        return -1;

    return 0;
}

//判断事件是否是 活跃的
int _uev_watcher_active(struct uev_st *w)
{
    if(!w) return 0;

    return w->active > 0;
}

int _uev_watcher_rearm(struct uev_st *w)
{
    struct epoll_event ev;

    if(!w || w->fd < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ev.events = w->events | EPOLLRDHUP;
    ev.data.ptr = w;
    if(epoll_ctl(w->ctx->fd, EPOLL_CTL_MOD, w->fd, &ev) < 0) return -1;

    return 0;
}

//事件初始化
int uev_init(struct uev_ctx_st *ctx)
{
    //最大事件数量 10
    return uev_init1(ctx, MAX_EV_EVNETS);
}

//事件初始化
int uev_init1(struct uev_ctx_st *ctx, int max_events)
{
    if(!ctx || max_events < 1)
    {
        errno = EINVAL;
        return -1;
    }

    if(max_events > MAX_EV_EVNETS)
        max_events = MAX_EV_EVNETS;

    memset(ctx, 0, sizeof(*ctx));
    ctx->max_events = max_events;

    return _init(ctx, 0);   //不关闭老的 fd
}

//事件退出
int uev_exit(struct uev_ctx_st *ctx)
{
    struct uev_st *w;

    if(!ctx)
    {
        errno = EINVAL;
        return -1;
    }

    //遍历观察的事件
    _UEV_FOREACH(w, ctx->watchers)
    {
        _UEV_REMOVE(w, ctx->watchers);

        if(!_uev_watcher_active(w)) //判断事件是否是激活的
            continue;
        
        switch (w->type)
        {
        case UEV_IO_TYPE:
            uev_io_stop(w);
            break;
        case UEV_SIGNAL_TYPE:
            uev_signal_stop(w);
            break;
        case UEV_TIMER_TYPE:
        case UEV_CRON_TYPE:
            uev_timer_stop(w);
            break;
        case UEV_EVENT_TYPE:
            uev_event_stop(w);
            break;
        default:
            //TODO 日志类型
            break;
        }
    }

    ctx->watchers = NULL;   //监听的时间
    ctx->running = 0;   //运行状态
    if(ctx->fd > -1)
        close(ctx->fd);
    ctx->fd = -1;

    return 0;
}

//事件执行
int uev_run(struct uev_ctx_st *ctx, int flags)
{
    int timeout = -1;
    struct uev_st *w;

    if(!ctx || ctx->fd < 0)
    {
        errno = EINVAL;
        return -1;
    }

    if(flags & UEV_NONBLOCK) timeout = 0;   //非阻塞

    ctx->running = 1;   //运行状态 - 执行

    //开始定时器
    _UEV_FOREACH(w, ctx->watchers) {
        if(UEV_CRON_TYPE == w->type)
            uev_cron_set(w, w->u.c.when, w->u.c.interval);
        if(UEV_TIMER_TYPE == w->type)
            uev_timer_set(w, w->u.t.timeout, w->u.t.period);
    }

    while(ctx->running && ctx->watchers) 
    {
        struct epoll_event ee[MAX_EV_EVNETS];
        int max_events = ctx->max_events;
        int i, nfds, rerun = 0;

        if(max_events > MAX_EV_EVNETS)
            max_events = MAX_EV_EVNETS;
        
        if(ctx->workaround) 
        {
            _UEV_FOREACH(w, ctx->watchers) 
            {
                if(w->active != -1 || !w->cb)
                    continue;
                
                if(!has_data(w->fd)) 
                {
                    w->active = 0;
                    _UEV_REMOVE(w, ctx->watchers);
                }

                rerun++;
                w->cb(w, w->args, UEV_READ);
            }
        }

        if(rerun)
            continue;
        ctx->workaround = 0;

        while((nfds = epoll_wait(ctx->fd, ee, max_events, timeout)) < 0) 
        {
            if(!ctx->running) break;    //如果事件停止则退出

            //EINTR Interrupted system call
            if(EINTR == errno) continue;    //Signalled, try again

            uev_exit(ctx);

            return -2;  //TODO 为啥是-2
        }

        for(i = 0; ctx->running && i < nfds; i++)
        {
            struct signalfd_siginfo fdsi;
            ssize_t sz = sizeof(fdsi);
            uint32_t events;
            uint64_t exp;

            w = (struct uev_st *)ee[i].data.ptr;   //为啥是这样
            events = ee[i].events;

            switch (w->type)
            {
            case UEV_IO_TYPE:
                if(events & (EPOLLHUP | EPOLLERR))
                    uev_io_stop(w);
                break;
            case UEV_SIGNAL_TYPE:   //如果是信号
                if(read(w->fd, &fdsi, sz) != sz) 
                {
                    if(uev_signal_start(w)) 
                    {
                        uev_signal_stop(w);

                        events = UEV_ERROR;
                    }
                } else 
                    w->siginfo = fdsi;
                break;
            case UEV_TIMER_TYPE:
                if(read(w->fd, &exp, sizeof(exp)) != sizeof(exp))
                {
                    uev_timer_stop(w);
                    events = UEV_ERROR;
                }

                if(!w->u.t.period)
                    w->u.t.timeout = 0;

                if(!w->u.t.timeout)
                    uev_timer_stop(w);
                break;
            case UEV_CRON_TYPE:
                if(read(w->fd, &exp, sizeof(exp)) != sizeof(exp))
                {
                    events = UEV_HUP;
                    //ECANCELED 这个错误码表示一个操作被取消，
                    //可能是由于调用了类似于pthread_cancel或者cancel等取消操作的系统调用导致的
                    if(errno != ECANCELED)
                    {
                        uev_cron_stop(w);
                        events = UEV_ERROR;
                    }
                }

                if(!w->u.c.interval)
                    w->u.c.when = 0;
                else
                    w->u.c.when += w->u.c.interval;
                
                if(!w->u.c.when)
                    uev_timer_stop(w);
                
                break;
            case UEV_EVENT_TYPE:
                if(read(w->fd, &exp, sizeof(exp)) != sizeof(exp))
                    events = UEV_HUP;
                break;
            default:
                // TODO 错误日志
                break;
            }

            if(w->cb)
                w->cb(w, w->args, events & UEV_EVENT_MASK);
        }

        if(flags & UEV_ONCE)
            break;
    }
    return 0;
}

