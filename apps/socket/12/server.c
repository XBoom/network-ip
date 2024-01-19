#include <unistd.h>

/**
 * @brief 创建子进程
 * @return 成功返回进程ID(父子进程ID都返回), 失败时返回-1
*/
pid_t fork(void);

#include <sys/wait.h>

/**
 * @brief 父进程主动请求获取子进程的返回值
 * 1. 
 * 2. 函数参数所指向的单元中还含有其他信息，需要宏进行分离
 *   WIFEXITED 子进程正常终止时返回 真 true
 *   WEXITSTATUS 返回子进程的返回值
*/
pid_t wait(int * statloc);


//wait 函数会引起程序阻塞

/**
 * pid 等待终止的目标子进程的ID,若传递-1，则与wait函数相同，可以等待任意子进程终止
 * statloc 与wait的含义相同
 * option WNOHANG,即使没有终止的子进程也不会进入阻塞状态，而是返回0并退出函数
 * 
*/
pid_t waitpid(pid_t pid, int *staloc, int options);




