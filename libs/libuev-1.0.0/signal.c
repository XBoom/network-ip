/* libuev - Micro event loop library
 *
 * Copyright (c) 2012  Flemming Madsen <flemming!madsen()madsensoft!dk>
 * Copyright (c) 2013  Joachim Nilsson <troglobit()gmail!com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <errno.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <unistd.h> /* close(), read() */

#include "uev.h"

/**
 * Create a signal watcher
 * @param ctx    A valid libuev context
 * @param w      Pointer to an uev_t watcher
 * @param cb     Timer callback
 * @param arg    Optional callback argument
 * @param signo  Signal to watch for
 *
 * @return POSIX OK(0) or non-zero with @param errno set on error.
 */
int uev_signal_init(uev_ctx_t *ctx, uev_t *w, uev_cb_t *cb, void *arg, int signo)
{
	int fd;
	sigset_t mask;

	/**
	 * @brief 初始化一个信号集 mask
	 * 尝试创建一个文件描述符 fd，用于接收信号。
	 * 第一个参数 -1 表示从当前进程继承其信号掩码。
	 * 第二个参数 &mask 是一个指向信号集的指针，指定了 signalfd 应该监控的信号。
	 * 第三个参数 SFD_NONBLOCK 使得 signalfd 创建的文件描述符以非阻塞方式打开
	 * */
	sigemptyset(&mask);
	fd = signalfd(-1, &mask, SFD_NONBLOCK);
	if (fd < 0)
		return -1;

	// 初始化事件
	if (uev_watcher_init(ctx, w, UEV_SIGNAL_TYPE, cb, arg, fd, UEV_READ))
		goto exit;

	// 开启监听信号
	if (uev_signal_set(w, signo))
	{
		uev_watcher_stop(w);
	exit:
		close(fd);
		return -1;
	}

	return 0;
}

/**
 * Reset a signal watcher
 * @param w      Watcher to reset
 * @param signo  New signal to watch for
 *
 * @return POSIX OK(0) or non-zero with @param errno set on error.
 */
int uev_signal_set(uev_t *w, int signo)
{
	sigset_t mask;

	/* Every watcher must be registered to a context */
	if (!w || !w->ctx)
	{
		errno = EINVAL;
		return -1;
	}

	/* Handle stopped signal watchers */
	if (w->fd < 0)
	{
		/* Remove from internal list */
		LIST_REMOVE(w, link);

		if (uev_signal_init(w->ctx, w, (uev_cb_t *)w->cb, w->arg, signo))
			return -1;
	}

	sigemptyset(&mask);
	sigaddset(&mask, signo);

	/* Block signals so that they aren't handled
	   according to their default dispositions */
	if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
		return -1;

	if (signalfd(w->fd, &mask, SFD_NONBLOCK) < 0)
		return -1;

	return uev_watcher_start(w);
}

/**
 * Stop a signal watcher
 * @param w  Watcher to stop
 *
 * @return POSIX OK(0) or non-zero with @param errno set on error.
 */
int uev_signal_stop(uev_t *w)
{
	if (uev_watcher_stop(w))
		return -1;

	close(w->fd);
	w->fd = -1;

	return 0;
}

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
