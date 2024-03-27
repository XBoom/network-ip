#include <sys/stat.h>
#include <stdlib.h> // EXIT_SUCCESS
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

    CHECK_RET_GOTO(setid() == -1, end, "setid failed");

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
end:
    return -1;
}
