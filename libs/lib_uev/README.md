### 参考链接
1. https://github.com/troglobit/libuev

```
/* Set up a timer watcher to call cb() every other second */
#include <stdio.h>
#include <uev/uev.h>

static void cb(uev_t *w, void *args, int events)
{
        if (UEV_ERROR == events) {
            puts("Problem with timer, attempting to restart.");
            uev_timer_start(w);
            return;
        }

        puts("Every other second");
}

int main(void)
{
        uev_ctx_t ctx;
        uev_t timer;

        uev_init(&ctx);
        uev_timer_init(&ctx, &timer, cb, NULL, 2 * 1000, 2 * 1000);

        return uev_run(&ctx, 0);
}
```