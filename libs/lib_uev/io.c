#include <errno.h>
#include "uev.h"

//io init
int uev_io_init(struct uev_ctx_st *ctx, struct uev_st *w, 
        uev_cb_t *cb, void *args, int fd, int events)
{
    if(fd < 0) 
    {
        errno = EINVAL;
        return -1;
    }

    //初始化
    if(_uev_watcher_init(ctx, w, 
        UEV_IO_TYPE, cb, args, fd, events)) return -1;


    return _uev_watcher_start(w);    
}

//reset io watcher
int uev_io_set(struct uev_st *w, int fd, int events)
{
    if((events & UEV_ONESHOT) && _uev_watcher_active(w))
        return _uev_watcher_rearm(w);
    
    uev_io_stop(w);

    return uev_io_init(w->ctx, w, (uev_cb_t *)w->cb, w->args, fd, events);
}


int uev_io_start(struct uev_st *w)
{
    return uev_io_set(w, w->fd, w->events);
}

int uev_io_stop(struct uev_st *w)
{
    return _uev_watcher_stop(w);
}

