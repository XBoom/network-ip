#include "common.h"
#include "ipc.h"

#define CHILD_PROC_NUM (5)
#define MAX_BUF_SIZE (1024)

int main(int argc, char *argv[])
{
    int ret = 0;
    int fds[2] = {0};
    ret = pipe(fds);
    CHECK_RET(ret != 0, "pipe failed");

    for(int i = 0; i < CHILD_PROC_NUM; i++)
    {
        pid_t pid = fork();
        CHECK_RET_EXIT(pid < 0, "fork failed");
        if(pid == 0)
        {
            close(fds[1]);  //关闭复制的无用的管道读端
            int temp = 0;
            read(fds[0], &temp, 1);
            LOG_INFO("child %d read end", getpid());
            close(fds[0]);  //写完之后关闭写入端
            return 0;
        } 
    }
    sleep(5);
    LOG_INFO("program exit");
    close(fds[1]);  //关闭管道写端
    return 0;
}
