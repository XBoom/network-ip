#include <errno.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "uev.h"

int uev_event_init(struct uev_ctx_st *ctx, struct uev_st *w, 
    uev_cb_t *cb, void *args)
{
    int fd;

    if(!w || !ctx)
    {
        errno = EINVAL;
        return -1;
    }

    w->fd = -1;
    fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(fd < 0)
        return -1;
    
    return _uev_watcher_init(ctx, w, UEV_EVENT_TYPE, cb, args, fd, UEV_READ)
        || _uev_watcher_start(w);
}

//发送事件
int uev_event_post(struct uev_st* w)
{
    uint64_t val = 1;

    if(!w || -1 == w->fd)
    {
        errno = EINVAL;
        return -1;
    }

    if(write(w->fd, &val, sizeof(val)) != sizeof(val))
        return -1;
    
    return 0;
}

//停止事件
int uev_event_stop(struct uev_st *w)
{
    if(!_uev_watcher_active(w))
        return 0;
    
    if(_uev_watcher_stop(w))
        return -1;
    
    close(w->fd);
    w->fd = -1;

    return 0;
}