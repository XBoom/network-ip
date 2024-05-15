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
#include <string.h> /* memset() */
#include <sys/epoll.h>
#include <sys/signalfd.h> /* struct signalfd_siginfo */
#include <unistd.h>		  /* close(), read() */

#include "uev.h"

// 通过 __attribute__((unused)) 被明确标记为未使用，
// 这样当使用 GCC 或支持此特性的编译器编译时，就不会因为未使用该参数而发出警告
#define UNUSED(arg) arg __attribute__((unused))

/* 事件观察者初始化 */
int uev_watcher_init(uev_ctx_t *ctx, uev_t *w, uev_type_t type, uev_cb_t *cb, void *arg, int fd, int events)
{
	if (!ctx || !w)
	{
		errno = EINVAL;
		return -1;
	}

	memset(w, 0, sizeof(uev_t));
	w->ctx = ctx;
	w->fd = fd;
	w->type = type;
	w->cb = (void *)cb;
	w->arg = arg;
	w->events = events;

	LIST_INSERT_HEAD(&w->ctx->watchers, w, link);

	return 0;
}

/* 事件观察者 开始监听事件(添加到epoll中)*/
int uev_watcher_start(uev_t *w)
{
	struct epoll_event ev;

	if (!w || w->fd < 0)
	{
		errno = EINVAL;
		return -1;
	}

	if (w->active)
		return 0;

	ev.events = w->events;
	ev.data.ptr = w;
	if (epoll_ctl(w->ctx->fd, EPOLL_CTL_ADD, w->fd, &ev) < 0)
		return -1;

	w->active = 1;

	return 0;
}

/* 事件观察者 停止观察某件事 */
int uev_watcher_stop(uev_t *w)
{
	if (!w)
	{
		errno = EINVAL;
		return -1;
	}

	if (!w->active)
		return 0;

	/* Remove from kernel */
	epoll_ctl(w->ctx->fd, EPOLL_CTL_DEL, w->fd, NULL);
	w->active = 0;

	return 0;
}

/**
 * @brief 初始化事件上下文(创建epoll、初始化观察者链表)
 */
int uev_init(uev_ctx_t *ctx)
{
	int fd;

	fd = epoll_create(1);
	if (fd < 0)
		return -1;

	memset(ctx, 0, sizeof(*ctx));
	ctx->fd = fd;
	LIST_INIT(&ctx->watchers);

	return 0;
}

/**
 * @brief 事件
 */
int uev_exit(uev_ctx_t *ctx)
{
	if (!ctx)
	{
		errno = EINVAL;
		return -1;
	}

	while (!LIST_EMPTY(&ctx->watchers))
	{
		uev_t *w = LIST_FIRST(&ctx->watchers);

		/* Remove from internal list */
		LIST_REMOVE(w, link);

		if (!w->active)
			continue;

		switch (w->type)
		{
		case UEV_TIMER_TYPE:
			uev_timer_stop(w);
			break;

		case UEV_SIGNAL_TYPE:
			uev_signal_stop(w);
			break;

		case UEV_IO_TYPE:
			uev_io_stop(w);
			break;
		}
	}

	ctx->running = 0;
	close(ctx->fd);
	ctx->fd = -1;

	return 0;
}

/**
 * Start the event loop
 * @param ctx    A valid libuev context
 * @param flags  A mask of %UEV_ONCE and %UEV_NONBLOCK, or zero
 *
 * @return POSIX OK(0) upon successful termination of the event loop, or non-zero on error.
 */
int uev_run(uev_ctx_t *ctx, int flags)
{
	int result = 0, timeout = -1;
	uev_t *w;

	if (!ctx || ctx->fd < 0)
	{
		errno = EINVAL;
		return -1;
	}

	if (flags & UEV_NONBLOCK)
		timeout = 0;

	/* Start the event loop */
	ctx->running = 1;

	/* Start all dormant timers */
	LIST_FOREACH(w, &ctx->watchers, link)
	{
		if (UEV_TIMER_TYPE == w->type)
			uev_timer_set(w, w->timeout, w->period);
	}

	while (ctx->running)
	{
		int i, nfds;
		struct epoll_event events[UEV_MAX_EVENTS];

		while ((nfds = epoll_wait(ctx->fd, events, UEV_MAX_EVENTS, timeout)) < 0)
		{
			if (!ctx->running)
				break;

			if (EINTR == errno)
				continue; /* Signalled, try again */
		exit:
			result = -1;
			ctx->running = 0;
			break;
		}

		for (i = 0; ctx->running && i < nfds; i++)
		{
			w = (uev_t *)events[i].data.ptr;

			if (UEV_TIMER_TYPE == w->type)
			{
				uint64_t exp;

				if (read(w->fd, &exp, sizeof(exp)) != sizeof(exp))
					goto exit;

				if (!w->period)
					w->timeout = 0;
			}

			if (UEV_SIGNAL_TYPE == w->type)
			{
				struct signalfd_siginfo fdsi;
				ssize_t sz = sizeof(fdsi);

				if (read(w->fd, &fdsi, sz) != sz)
					goto exit;

				w->signo = fdsi.ssi_signo;
			}

			if (w->cb)
				w->cb(ctx, w, w->arg, w->events);

			if (UEV_TIMER_TYPE == w->type)
			{
				if (!w->timeout)
					uev_timer_stop(w);
			}
		}

		if ((flags & UEV_ONCE) || LIST_EMPTY(&ctx->watchers))
			break;
	}

	return result;
}

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */
