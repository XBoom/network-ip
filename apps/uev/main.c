#include <stdio.h>

#include "uev.h"

static void cb(struct uev_st *w, void *args, int events)
{
    if(UEV_ERROR == events)
    {
        puts("Problem with timer, attempting to restart.");
        uev_timer_start(w);
        return;
    }

    puts("Every other second");
}

int main()
{
    struct uev_ctx_st ctx;
    struct uev_st timer;

    uev_init(&ctx);

    uev_timer_init(&ctx, &timer, cb, NULL, 2 * 1000, 2*1000);

    return uev_run(&ctx, 0);
}