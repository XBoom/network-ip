/*
 * @Author: Yuan Kang 728081687@qq.com
 * @Date: 2023-12-07 22:30:46
 * @LastEditors: Yuan Kang 728081687@qq.com
 * @LastEditTime: 2023-12-07 22:56:19
 * @FilePath: \network-ip\libs\lib_euv\src\uev.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <errno.h>
#include <fcntl.h>		/* O_CLOEXEC */
#include <string.h>		/* memset() */
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/select.h>		/* for select() workaround */
#include <sys/signalfd.h>	/* struct signalfd_siginfo */
#include <unistd.h>		/* close(), read() */

#include "uev.h"

/**
 * @brief 事件上下文初始化
 * @param
 *  uev_ctx_st *ctx 事件上下文
 *  int close_old 是否关闭旧的
 * @return 
 *  0 成功; -1 异常
*/
static int _init(uev_ctx_st *ctx, int close_old)
{
    int fd;

    fd = epoll_create1(EPOLL_CLOEXEC);  //TODO 这个接口的具体作用
    if(fd < 0)
        return -1;  //TODO 添加错误日志

    if(close_old)   //是否关闭旧的
        close(ctx-fd);  //TODO 如果不关闭，旧的怎么办
    
    ctx->fd = fd;
    return 0;   
}

//判单是否有事件
static int has_data(int fd)
{
    struct timeval timeout = {0, 0};    //select 的超时时间

    fd_set fds; //句柄合集
    int n = 0;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /**
     *  int select(int n, fd_set* readfds, fd_set* writefds, fd_set* errorfds, struct timeval* timeout);
     *  n 是指集合中所有文件木舒服的最大值加1 ！！！！
     *  readfds 表示监视这些文件描述符的读变化，如果有一个可读，那么select就会返回一个大于0的值
     *  writefds  表示监视这些文件描述符的写变化，如果有一个可读，那么select就会返回一个大于0的值
     *  errorfds  表示监视文件错误异常
     *  timeout 根据timeout 判断是否超时，如果超时，则返回0；如果发生错误，则返回负数。
     *      如果传入 NULL，则处于阻塞状态，一直等到监视的文件描述符有变化为止
     *      如果传入 0秒0毫秒，则处于非阻塞状态，但是不管文件描述符是否变化，都会立即返回
     *      如果传入 大于0，就是等待的超时时间，在timeout内阻塞
     * 
     *  注意：
     *      readfds 和 timeout 在每次执行 select 前都要重新初始化
     *          readfds 不清空检测不到描述符变化
     *          timeout 每次都要初始化其值，否则会被默认初始化为0
    */
    if(select(1, &fds, NULL, NULL, &timeout) > 0)
        return ioctl(0, FINOREAD, &n) == 0 && n > 0;

    return 0;
}

//事件观察者初始化
int _uev_watcher_init(uev_ctx_st *ctx, uev_t *w, uev_type_t type, uev_cb_t *cb, void * data, int fd, int events)
{
    if(NULL == ctx || NULL = w) 
    {
        errno = EINVAL;
        return -1;
    }

    w->ctx    = ctx;
	w->type   = type;
	w->active = 0;      // TODO 使用枚举类型代替
	w->fd     = fd;
	w->cb     = cb;
	w->arg    = data;T
	w->events = events;

    return 0;
}

int _uev_watcher_start(uev_t *w)
{   
    struct epoll_event ev;
}