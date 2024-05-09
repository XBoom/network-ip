#include <sys/stat.h>
#include <stdlib.h> // EXIT_SUCCESS
#include <unistd.h> // _SC_OPEN_MAX
#include <fcntl.h>  // O_RWDR
#include "daemon.h"

#include "utils.h"

/*
创建守护进程的一般步骤包括:
1. 在父进程中执行 fork() 并立即 exit()，以确保工作在子进程中进行，并且形式上脱离了控制终端。
2. 在子进程中调用 setsid() 来创建新的会话，使子进程完全独立出来，脱离控制。
3. 改变当前工作目录，通常是切换到根目录 /，以避免文件系统不能卸载的问题。
4. 重设文件创建掩码 umask(0)，以防止继承的文件创建屏蔽字拒绝某些权限。
5. 关闭所有不必要的文件描述符，以释放资源并避免潜在的资源泄露
*/

/**
 * @brief 守护进程初始化
 */
int daemon_init(int flags)
{
    int max_fd, fd;
    switch (fork())
    {
    case -1:
        return -1; // 异常退出
    case 0:
        break; // 子进程
    default:
        _exit(EXIT_SUCCESS); // 父进程终止
    }

    CHECK_RET_GOTO(setsid() == -1, end, "setid failed");

    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0);

    if (!(flags & BD_NO_CHDIR))
        chdir("/");

    if (!(flags & BD_NO_CLOSE_FILES))
    {
        max_fd = sysconf(_SC_OPEN_MAX);
        if (max_fd == -1)
            max_fd = BD_MAX_CLOSE;

        for (fd = 0; fd < max_fd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS))
    {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)
            return -1;

        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;

        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
end:
    return -1;
}

int main(int argc, char *argv[])
{
    daemon_init(0);
    sleep(20000);
    LOG_INFO("daemon exit");
    return 0;
}

/*
系统关闭的时候 init 进程会向所有其子进程发送这个信号。
在默认情况下，SIGTERM 信号会终止一个进程。
如果 daemon 在终止之前需要做些清理工作，那么就需要为这个信号建立一个处理器

而且处理器必须能快速地完成清理工作，
因为 init 在发完 SIGTERM 信号的 5 秒之后会发送一个 SIGKILL 信号

这并不意味着这个
daemon 能够执行 5 秒的 CPU 时间，因为 init 会同时向系统中的所有进程发送信号，而它们可
能都试图在 5 秒内完成清理工作
*/
