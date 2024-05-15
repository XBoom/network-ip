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

    for (int i = 0; i < CHILD_PROC_NUM; i++)
    {
        pid_t pid = fork();
        CHECK_RET_EXIT(pid < 0, "fork failed");
        if (pid == 0)
        {
            srand(time(NULL) + getpid());

            close(fds[0]); // 关闭复制的无用的管道读端

            int seconds = rand() % 6;
            sleep(seconds);
            LOG_INFO("child %d sleep %d to close pipe", getpid(), seconds);
            close(fds[1]); // 写完之后关闭写入端
            return 0;
        }
    }
    close(fds[1]); // 父进程关闭无用的管道写端

    char msg[MAX_BUF_SIZE] = {0};
    ret = read(fds[0], msg, MAX_BUF_SIZE);

    LOG_INFO("program exit %d", ret);
    return 0;
}
