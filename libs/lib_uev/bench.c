#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "uev.h"

#ifdef CTEST
typedef struct {
    int index;
}myarg_t;

static int num_pipes, num_active, num_writes;
static int timers, count, writes, fired;
static myarg_t *m_args;
static int *pipes;
static struct uev_st *evio;
static struct uev_st *evto;

static void read_cb(struct uev_st *w, void *args, int events)
{
    int idx, widx;
    u_char ch;
    myarg_t *m = args;

    idx = m->index;
    widx = idx + 1;
    if(timers)
    {
        //drand48() 是一个C标准库函数，用于生成一个[0.0, 1.0)范围内的伪随机双精度浮点数。
        uev_timer_set(&evto[idx], 10000 + drand48() * 1000, 0);
    }

    count += read(w->fd, &ch, sizeof(ch));
    if(writes)
    {
        if(widx >= num_pipes)
            widx -= num_pipes;
        
        if(write(pipes[2 * widx + 1], "e", 1) < 0)
        {
            perror("write()");
            abort();
        }

        writes--;
        fired++;
    }
}

static void timer_cb(struct uev_st *w, void *args, int events)
{
    /* nop */
}

static struct timeval *run_once(struct uev_ctx_st *ctx)
{
    int *cp, i, space;
    static struct timeval ta, ts, te;

    gettimeofday(&ta, NULL);
    for(cp = pipes, i = 0; i < num_pipes; i++, cp += 2)
    {
        uev_io_set(&evio[i], cp[0], UEV_READ);

        if(timers)
            uev_timer_set(&evto[i], 10000 + drand48() * 1000, 0);
    }

    uev_run(ctx, UEV_ONCE | UEV_NONBLOCK);

    fired = 0;
    space = num_pipes / num_active;
    space = space * 2;
    for(i = 0; i < num_active; i++, fired++)
    {
        if(write(pipes[i * space + 1], "e", 1) < 0)
        {
            perror("write()");
            abort();
        }
    }

    count = 0;
    writes = num_writes;
    {
        int xcount = 0;

        gettimeofday(&ts, NULL);

        do {
            uev_run(ctx, UEV_ONCE | UEV_NONBLOCK);
            xcount++;
        }while(count != fired);

        gettimeofday(&te, NULL);

        if(xcount != count)
            fprintf(stderr, "Xcount: %d, Rcount: %d\n", xcount, count);
    }

    timersub(&te, &ta, &ta);
    timersub(&ts, &ta, &ta);
    fprintf(stdout, "%8ld %8ld \n",
        te.tv_sec * 1000000L + te.tv_usec,
        ts.tv_sec * 1000000L + ts.tv_usec);

    return &te;
    
};


int main(int argc, char **argv)
{
    struct rlimit rl;
    int i, c;
    int *cp;
    struct uev_ctx_st *ctx;
    extern char *optarg;

    num_pipes = 100;
    num_active = 1;
    num_writes = num_pipes;
    while((c = getopt(argc, argv, "a:n:tw:")) != -1)
    {
        switch (c)
        {
        case 'a':
            num_active = atoi(optarg);
            break;
        case 'n':
            num_pipes = atoi(optarg);
            break;
        case 't':
            timers = 1;
            break;
        case 'w':
            num_writes = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Illegal argument \"%c\"\n", c);
            break;
        }
    }

    /*
        resource 参数指定要设置的资源限制的类型，这里是 RLIMIT_NOFILE，表示文件描述符的限制。
        struct rlimit 结构体的指针，该结构体包含软限制和硬限制的值。

        struct rlimit {
            rlim_t rlim_cur;  // 软限制
            rlim_t rlim_max;  // 硬限制
        };
    */
    rl.rlim_cur = rl.rlim_max = num_pipes * 3 + 50;
    if(setrlimit(RLIMIT_NOFILE, &rl) == -1)
    {
        perror("setrlimit");
        return 1;
    }

    m_args = calloc(num_pipes, sizeof(myarg_t));
    evio = calloc(num_pipes, sizeof(struct uev_st));
    evto = calloc(num_pipes, sizeof(struct uev_st));
    pipes = calloc(num_pipes * 2, sizeof(int));
    ctx = calloc(1, sizeof(struct uev_ctx_st));
    if(!m_args || !evio || !evto || !pipes)
    {
        perror("calloc");
        return 1;
    }

    uev_init(ctx);

    for(cp = pipes, i = 0; i < num_pipes; i++, cp += 2)
    {
        if(timers)
            uev_timer_init(ctx, &evto[i], timer_cb, NULL, 0, 0);
        m_args[i].index = i;

#ifdef USE_PIPES
        if(pipe2(cp, O_NONBLOCK) == -1)
        {
#else
        if(socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, cp) == -1) 
        {
#endif
            perror("pipe");
            exit(1);
        }

        uev_io_init(ctx, &evio[i], read_cb, &m_args[i], cp[0], UEV_READ);
    }

    for(i = 0; i < 2; i++)
        run_once(ctx);
    
    return 0;
}

#endif