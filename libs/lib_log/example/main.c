//#include "uev.h"
#include "common.h"

// static void cb(struct uev_st *w, void *args, int events)
// {
//     if(UEV_ERROR == events)
//     {
//         puts("Problem with timer, attempting to restart.");
//         uev_timer_start(w);
//         return;
//     }

//     puts("Every other second");
// }

static void show_log()
{
    LOG_INFO("log info %s", "hello");
    LOG_ERROR("log error %s", "hello");
    LOG_WARN("log warn %s", "hello");
    LOG_DEBUG(DL_1, "log debug %s", "hello");
    LOG_DEBUG(DL_2, "log debug 2 %s", "hello");
    REAL_ERROR("real error");
    REAL_INFO("real info");
    
    uint32_t ret = LOG_GET_LEVEL();
    REAL_INFO("log lev %d", ret);

    LOG_SET_LEVEL(DL_2);
    LOG_INFO("log info %s", "hello");
    LOG_ERROR("log error %s", "hello");
    LOG_WARN("log warn %s", "hello");
    LOG_DEBUG(DL_1, "log debug %s", "hello");
    LOG_DEBUG(DL_2, "log debug 2 %s", "hello");
    REAL_ERROR("real error");
    REAL_INFO("real info");
}

int main()
{
    show_log();

    // struct uev_ctx_st ctx;
    // struct uev_st timer;

    // uev_init(&ctx);

    // uev_timer_init(&ctx, &timer, cb, NULL, 2 * 1000, 2*1000);

    // return uev_run(&ctx, 0);
}