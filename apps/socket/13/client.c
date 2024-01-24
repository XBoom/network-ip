#include "common.h"

//父进程等待子进程

int main(int argc, char *argv[])
{
    int child_pid = fork(); //构建一个子进程
    CHECK_RET(child_pid < 0, "fork failed");
    if(child_pid == 0)  //child
    {
        sleep(5000);    
        LOG_INFO("child end");
        exit(7);
    } else 
    {
        int status = 0;
        while(!waitpid(child_pid, &status, WHOHANG))
        {
            sleep(500);
            LOG_INFO("wait child");
        } //进程不会阻塞
        if(WIFEXITED(status))
            LOG_INFO("child has exited %d", WEXITSTATUS(status));

        LOG_INFO("child pid %d status %d", child_pid, WEXITSTATUS(status));
    }
    return 0;
}