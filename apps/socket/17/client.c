#include "common.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

// 表示文件所有者具有读写权限，所属组和其他用户只有读权限
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void child_proc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    LOG_INFO("child %d exit", pid);
}

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 1, "Usage: client");

    int ret = 0;

    return 0;
}