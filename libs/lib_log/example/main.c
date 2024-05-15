#include "common.h"

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
    return 0;
}