#include "common.h"
#include "person.pb-c.h"
#include "info.h"

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

int main(int argc, char *argv[])
{
    Person p = PERSON__INIT;
    student stu = {0};

    LOG_INFO("proto person id %d %d", p.id, stu.age);
    show_log();
}