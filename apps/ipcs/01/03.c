#include "common.h"

int main(int argc, char *argv[])
{
    int fds[2];

    CHECK_RET_EXIT(pipe(fds), "pipe failed");

    int pid = fork();
    CHECK_RET_EXIT(pid < 0, "fork failed");

    if(pid == 0)    //这个子进程用来写
    {
        close(fds[0]);  //关闭无用的读端
        if(fds[1] != STDOUT_FILENO) //检查写端是否是输出
        {
            //close desc 1, and reopen boud to write end of pipe
            CHECK_RET_EXIT(dup2(fds[1], STDOUT_FILENO) == -1, "dup2 failed");
            //close(fds[1]);
        }
        execlp("ls", "ls", (char *)NULL);
        return 0;
    }

    //构建第二个子进程
    int pid2 = fork();
    CHECK_RET_EXIT(pid2 < 0, "fork failed");
    if(pid2 == 0)
    {
        close(fds[1]);
        if(fds[0] != STDIN_FILENO)
        {
            CHECK_RET_EXIT(dup2(fds[0], STDIN_FILENO) == -1, "dup2 failed");
            //close(fds[0]);
        }
        execlp("wc", "wc", "-l", (char *)NULL); 
        return 0;
    }

    close(fds[0]);
    close(fds[1]);
    //等待两个子进程结束
    wait(NULL);
    wait(NULL);
    return 0;
}