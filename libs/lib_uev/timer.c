#include <errno.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "uev.h"

static void msec2ttspec(int msec, struct timespec *ts)
{
    if(msec)
    {
        ts->tv_sec = msec / 1000;
        ts->tv_nsec = (msec % 1000) * 1000000;
    } else
    {
        ts->tv_sec = 0;
        ts->tv_nsec = 0;
    }    
}

//定时器事件开始
int uev_timer_init(struct uev_ctx_st *ctx, struct uev_st *w, uev_cb_t *cb, void *args, int timeout, int period)
{
    int fd;

    if(timeout < 0 || period < 0)
    {
        errno = ERANGE;
        return -1;
    }

    /**
     *  #include <sys/timerfd.h>
     *  int timerfd_create(int clockid, int flags);
     *  clockid 参数指定了计时钟的类型，可以是以下之一：
     *      CLOCK_REALTIME: 使用系统实时时钟，可以通过系统时间设置。(常用于 timer)
     *      CLOCK_MONOTONIC: 使用系统单调时钟，不受系统时间调整的影响。(常用于 cron)
     *      其他可能的值，具体取决于系统支持的计时钟类型。
     *  flags 参数是一组标志，可以使用位掩码（OR 操作）组合多个标志。常用的标志包括：
     *      TFD_NONBLOCK: 将定时器描述符设置为非阻塞模式。
     *      TFD_CLOEXEC: 在 exec 调用中关闭定时器描述符
    */
    fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(fd < 0)
        return -1;
    
    if(_uev_watcher_init(ctx, w, UEV_TIMER_TYPE, cb, args, fd, UEV_READ))
        goto exit;
    
    if(uev_timer_set(w, timeout, period))
    {
        _uev_watcher_stop(w);
    exit:
        close(fd);
        w->fd = -1;
        return -1;
    }
    return 0;
}

int uev_timer_set(struct uev_st *w, int timeout, int period)
{
    if(!w || !w->fd)
    {
        //errno 是一个全局的错误码变量，用于表示发生在程序中的系统调用或库函数中的错误
        errno = EINVAL;
        return -1;
    }

    if(timeout < 0 || period < 0)
    {
        errno = ERANGE;
        return -1;
    }

    if(w->fd < 0)
    {
        if(!timeout && !period) return 0;

        if(uev_timer_init(w->ctx, w, (uev_cb_t *)w->cb, w->args, timeout, period))
            return -1;
    }

    w->u.t.timeout = timeout;
    w->u.t.period = period;

    if(w->ctx->running)
    {
        struct itimerspec time;

        msec2ttspec(timeout, &time.it_value);
        msec2ttspec(period, &time.it_interval);
        /*
            // 配置定时器时间
            struct itimerspec new_value;
            new_value.it_value.tv_sec = 5;      // 初始定时器到期时间：5秒后
            new_value.it_value.tv_nsec = 0;
            new_value.it_interval.tv_sec = 2;   // 之后每2秒触发一次定时器
            new_value.it_interval.tv_nsec = 0;
        */
        if(timerfd_settime(w->fd, 0, &time, NULL) < 0)
            return 1;
    }

    return _uev_watcher_start(w);
}

//定时器开始
int uev_timer_start(struct uev_st *w)
{
    if(!w)
    {
        errno = EINVAL;
        return -1;
    }

    if(-1 != w->fd)
        _uev_watcher_stop(w);
    
    return uev_timer_set(w, w->u.t.timeout, w->u.t.period);
}

//定时器关闭
int uev_timer_stop(struct uev_st *w)
{
    if(!_uev_watcher_active(w))
        return 0;
    
    if(_uev_watcher_stop(w))
        return -1;

    close(w->fd);
    w->fd = -1;

    return 0;
}