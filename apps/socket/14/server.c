#include <signal.h>

/**
* 返回类型：void (*)(int)，表示返回一个指向参数为整型、返回类型为 void 的函数的指针
* 函数名：signal，是函数的名称。
* 参数列表：(int signo, void (*func)(int))，包括两个参数：
    signo：整型，表示信号的编号。
    (*func)(int)：一个指向参数为整型、返回类型为 void 的函数的指针，用于指定处理该信号时要调用的函数
*/
//void (*signal(int signo, void (*func)(int)))(int);


/**
 * signo 信号信息
 * act 对应与第一个参数的信号处理函数
 * oldact 通过次参数获取之前注册的信号处理函数指针，若不需要则传递 0
*/
// int sigaction(int signo, const struct sigaction *act, struct sigaction * oldact);

// struct  sigaction {
// 	union __sigaction_u __sigaction_u;  /* signal handler */
// 	sigset_t sa_mask;               /* signal mask to apply */
// 	int     sa_flags;               /* see signal options below */
// };


/**
常用的处理信号有
SIGALRM: 已经通过调用alarm函数注册的时间
         通过 alarm 函数注册的时间表示：在接收到 SIGALRM 信号之前的秒数。当调用 alarm(seconds) 后，系统会在指定的秒数后发送 SIGALRM 信号给调用进程
SIGINT: 输入 CTRL + C
SIGCHLD: 子进程终止
*/


//31#include <unistd.h>

//返回 0 或者已秒为单位的距 SIGALRM 信号发生所剩实践
//unsigned int alarm(unsigned int seconds);

int main()
{
    return 0;
}