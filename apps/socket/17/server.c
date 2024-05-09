#include "common.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

//表示文件所有者具有读写权限，所属组和其他用户只有读权限
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

    int32_t ret = 0;
    pid_t child_pid;
    int read_fd = 0;
    int write_fd = 0;

    //注册型号退出
    struct sigaction act;
    act.sig_handler = child_proc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    ret = mkfifo(FIFO1, FILE_MODE);
    CHECK_RET(ret < 0 && errno != EEXIST, "can't create %s", FIFO1);

    ret = mkfifo(FIFO2, FILE_MODE);
    CHECK_RET(ret < 0 && errno != EEXIST, "can't create %s", FIFO2);

    child_pid = fork();
    if(child_pid == 0)
    {
        read_fd = open(FIFO1, O_RDONLY, 0);
        write_fd = open(FIFO2, O_WRONLY, 0);

        return 0;
    }
    
    write_fd = open(FIFO1, O_WRONLY, 0);
    read_fd = open(FIFO2, O_RDONLY, 0);

    return 0;
}