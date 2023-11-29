### 参考链接
1. https://github.com/nikhilm/uvbook
2. https://docs.libuv.org/en/v1.x/guide.html
3. https://github.com/libuv/libuv

```
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

int main() {
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}
```