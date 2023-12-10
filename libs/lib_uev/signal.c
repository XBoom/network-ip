#include <errno.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <unistd.h>

#include "uev.h"

int uev_signal_init(struct uev_ctx_st *ctx, struct uev_st *w, 
    uev_cb_t *cb, void *args, int signo)
{
    sigset_t mask;
    int fd;

    if(!w || !ctx)
    {
        errno = EINVAL;
        return -1;
    }

    w->fd = -1;

    sigemptyset(&mask);
    fd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
    if(fd < 0)
        return -1;

    if(_uev_watcher_init(ctx, w, UEV_SIGNAL_TYPE, cb, args, fd, UEV_READ))
        goto exit;
    
    if(uev_signal_set(w, signo))
    {
        _uev_watcher_stop(w);
    exit:
        close(fd);
        return - 1;
    }

    return 0;
}

int uev_signal_set(struct uev_st *w, int signo)
{
    sigset_t mask;

    if(!w || !w->ctx)
    {
        errno = EINVAL;
        return -1;
    }

    w->signo = signo;

    if(w->fd < 0)
    {
        if(uev_signal_init(w->ctx, w, (uev_cb_t *)w->cb, w->args, signo))
            return -1;
    }

    sigemptyset(&mask);
    sigaddset(&mask, signo);

    if(sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        return -1;
    
    if(signalfd(w->fd, &mask, SFD_NONBLOCK) < 0)
        return -1;
    
    return _uev_watcher_start(w);
}

int uev_signal_start(struct uev_st *w)
{
    if(!w)
    {
        errno = EINVAL;
        return -1;
    }

    if(w->fd == -1)
        uev_signal_stop(w);
    
    return uev_signal_set(w, w->signo);
}

int uev_signal_stop(struct uev_st *w)
{
    if(!_uev_watcher_active(w))
        return 0;
    
    if(_uev_watcher_stop(w))
        return -1;
    
    close(w->fd);
    w->fd = -1;
    
    return 0;
}