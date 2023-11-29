#include <errno.h>
#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "uev.h"

#ifndef TFD_TIMER_CANCEL_ON_SET
#define TFD_TIMER_CANCEL_ON_SET (1 << 1)
#endif

// TFD_TIMER_ABSTIME: 该标志表示在设置 TFD 定时器时使用绝对时间
// TFD_TIMER_CANCEL_ON_SET: 该标志表示在设置 TFD 定时器时，如果定时器已经存在，则取消先前的定时器。
#ifndef TFD_SETTING_FLAGS
#define TFD_SETTING_FLAGS (TFD_TIMER_ABSTIME | TFD_TIMER_CANCEL_ON_SET)
#endif

int uev_cron_init(struct uev_ctx_st *ctx, struct uev_st *w, 
        uev_cb_t *cb, void *args, time_t when, time_t interval)
{
    int fd;

    //参数不对
    if(when < 0 || interval < 0)
    {
        errno = ERANGE;
        return -1;
    }

    fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
    if(fd < 0)
    {
        return -1;
    }

    if(_uev_watcher_init(ctx, w, UEV_CRON_TYPE, cb, args, fd, UEV_READ))
        goto exit;

    if(uev_cron_set(w, when, interval))
    {
        _uev_watcher_stop(w);

    exit:
        close(fd);
        return -1;
    }

    return 0;
}

int uev_cron_set(struct uev_st* w, time_t when, time_t interval)
{
    if(!w || !w->ctx)
    {
        errno = EINVAL;
        return -1;
    }

    if(when < 0 || interval < 0)
    {
        errno = ERANGE;
        return -1;
    }

    if(w->fd < 0)
    {
        if(!when && !interval) return 0;

        if(uev_cron_init(w->ctx, w, (uev_cb_t *)w->cb, w->args, when, interval))
            return -1;
    }

    w->u.c.when = when;
    w->u.c.interval = interval;
    
    if(w->ctx->running)
    {
        struct itimerspec time;
        time.it_value.tv_sec = when;
        time.it_interval.tv_sec = interval;
        if(timerfd_settime(w->fd, TFD_SETTING_FLAGS, &time, NULL) < 0)
            return 1;
    }

    return _uev_watcher_start(w);
}

int uev_cron_start(struct uev_st *w)
{
    return uev_timer_start(w);
}

int uev_cron_stop(struct uev_st *w)
{
    return uev_timer_stop(w);
}