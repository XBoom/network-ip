#include "common.h"
#include "ipc.h"

/**
 *  @brief 创建一个新共享内存或获取一个既有段的标识符
 *  @param
 *      key: 共享内存键值，用于标识共享内存。可以是整数或字符串。
 *      size: 共享内存的大小，以字节为单位。
 *      shmflg: 共享内存标志，可以是以下值之一：
 *          IPC_CREAT: 如果不存在就创建一个新的共享内存。
 *          IPC_EXCL: 创建新的共享内存，如果已存在则报错。
 *          SHM_HUGETLB: (自 Linux2.6 起)，创建一个使用巨页的共享内存段
 *          SHM_NORESERVE:
 *  @return
 *      成功则返回新的或即有共享内存段的标识符
 *
 */
int shmget(key_t key, size_t size, int shmflg);

/**
 * @brief 将 shmid 标识的共享内存段附加到调用进程的虚拟地址空间中
 * @param
 *    如果 shmaddr 是 NULL，那么段会被附加到内核所选择的一个合适的地址处。(推荐)
 *    如果 shmaddr 不为 NULL 并且没有设置 SHM_RND，那么段会被附加到由 shmaddr 指
定的地址处，它必须是系统分页大小的一个倍数（否则会发生 EINVAL 错误）。
* 如果 shmaddr 不为 NULL 并且设置了 SHM_RND，那么段会被映射到的地址为在
shmaddr 中提供的地址被舍入到最近的常量 SHMLBA（shared memory low boundary
address）的倍数。这个常量等于系统分页大小的某个倍数。将一个段附加到值为
SHMLBA 的倍数的地址处在一些架构上是有必要的，因为这样才能够提升 CPU 的快
速缓冲性能和防止出现同一个段的不同附加操作在 CPU 快速缓冲中存在不一致的视
图的情况。
    shmflg
        SHM_RDONLY  附加只读段
        SHM_REMAP   天环卫浴 shmaddr 处任意即有映射
        SHM_RND 将 shmaddr 四舍五入为 SHMLBA 字节的倍数
 */
void *shmat(int shmid, const void *shmaddr, int shmflg);

/*
通过 fork()创建的子进程会继承其父进程附加的共享内存段。因此，共享内存为父进程和
子进程之间的通信提供了一种简单的 IPC 方法。
在一个 exec()中，所有附加的共享内存段都会被分离。在进程终止之后共享内存段也会自
动被分离
*/
int shmdt(const void *shmaddr);
