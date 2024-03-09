#include "common.h"

#define MAX_BUF_SIZE (30)

int main(int argc, char *argv[])
{
    int ret = 0;
    int fds[2];
    char str[] = "Who are you?";
    char buf[MAX_BUF_SIZE] = {0};

    pid_t child_pid;

    ret = pipe(fds);
    CHECK_RET(ret != 0, "pipe failed");

    child_pid = fork();
    CHECK_RET(child_pid < 0, "fork failed");

    if(child_pid == 0)
    {
        close(fds[0]);
        write(fds[1], str, sizeof(str));
    }
    else
    {
        close(fds[1]);
        read(fds[0], buf, MAX_BUF_SIZE);
        LOG_INFO("parent recv %s", buf);
    }
    return 0;
}