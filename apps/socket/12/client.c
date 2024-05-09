#include "common.h"

static int g_num = 0;
int g_num_2 = 0;
int main(int argc, char *argv[])
{
    int a = 0;
    int ptr_num = 1;
    int *p = &ptr_num;
    int child_pid = fork();
    if(child_pid < 0)
    {
        LOG_ERROR("fork failed");
        return -1;
    } 
    else if(child_pid == 0) //父进程
    {
        int b = 0;
        LOG_INFO("parent before %d %d %d %d", g_num, g_num_2, a, b);
        g_num += 2;
        g_num_2 += 2;
        a += 2;
        b += 2;
        LOG_INFO("parent after %d %d %d %d %p", g_num, g_num_2, a, b, p);
    } 
    else 
    {
        sleep(1);
        int b = 0;
        LOG_INFO("son before %d %d %d %d", g_num, g_num_2, a, b);
        g_num += 3;
        g_num_2 += 3;
        a += 3;
        b += 3;
        LOG_INFO("son after %d %d %d %d %p", g_num, g_num_2, a, b, p);
    }
    //LOG_INFO("main after %d %d %d", g_num, a, b); //'b' undeclared (first use in this function)LOG_INFO("main after %d %d %d", g_num, a, b)
    LOG_INFO("main after %d %d %d", g_num, g_num_2, a);
    return 0;
}