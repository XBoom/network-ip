#ifndef __SYSV_MSG_H__
#define __SYSV_MSG_H__
#include <sys/types.h>
#include <sys/msg.h>

/**
 * @brief
 * msgflag
 *  1. 如果不存在则创建
 *  2. 如果存在则使用
 *  3. 如果不存在则退出
 */
int msgget(key_t key, int msgflg);


/**
* 必须要将消息结构中的 mtype 字段的值设为一个大于 0 的值
* @param

    msgflag IPC_NOWAIT 执行一个非阻塞的发送操作。通常，当消息队列满时，msgsnd()会阻塞直到队列中有足够
的空间来存放这条消息。但如果指定了这个标记，那么 msgsnd()就会立即返回 EAGAIN 错误
* @return 0 on success, or -1 on error(不是返回字节数)
*/
int msgsend(int msqid, const void *msgp, size_t msg_t msgsz, int msgflag);

/**
* @param
    maxmsgsz: msgp 缓冲区中 mtext 字段的最大可用空间是通过 maxmsgsz 参数来指定的
    msgtyp:
        如果 msgtyp 等于 0，那么会删除队列中的第一条消息并将其返回给调用进程
        如果 msgtyp 大于 0，那么会将队列中第一条 mtype 等于 msgtyp 的消息删除并将其返
回给调用进程(也就是可以存在多条相同的mtype, 如果每个进程指定一个id，那样就能互相不影响)
        如果 msgtyp 小于 0，那么就会将等待消息当成优先队列来处理。队列中 mtype 最小并
且其值小于或等于 msgtyp 的绝对值的第一条消息会被删除并返回给调用进程。
    msgflag:
        IPC_NOWAIT, 执行一个非阻塞接收。通常如果队列中没有匹配 msgtyp 的消息，那么 msgrcv()会阻塞直
到队列中存在匹配的消息为止.指定 IPC_NOWAIT 标记会导致 msgrcv()立即返回 ENOMSG
错误    
        MSG_EXCEPT, 即将队列中第一条 mtype 不等于 msgtyp 的消息删除并将其返回给调用者
        MSG_NOERROR,在默认情况下，当消息的 mtext 字段的大小超过了可用空间时（由 maxmsgsz 参数定
义），msgrcv()调用会失败。如果指定了 MSG_NOERROR 标记，那么 msgrcv()将会从队列中
删除消息并将其 mtext 字段的大小截短为maxmsgsz 字节，然后将消息返回给调用者。被截去的数据将会丢失

* @return returns nums of bytes copied into mtext field, or -1 on error
*/
ssize_t msgrecv(int msqid, void *msgp, size_t maxmsgsz, long msgtyp, int msgflag);


//消息队列控制
/**
* IPC_RMID 立即删除消息队列对象及其关联的 msqid_ds 数据结构。队列中所有剩余的消息都会丢失，
所有被阻塞的读者和写者进程会立即醒来，msgsnd()和 msgrcv()会失败并返回错误 EIDRM。
这个操作会忽略传递给 msgctl()的第三个参数
    IPC_STAT 将与这个消息队列关联的 msqid_ds 数据结构的副本放到 buf 指向的缓冲区中。在 46.4 节
将会介绍 msqid_ds 结构。
    IPC_SET 使用buf指向的缓冲区提供的值更新与这个消息队列关联的msqid_ds数据结构中被选中的字段。
*/
int msgctl(int msqid, int cmd, struct msqid_ds *buf);

//每个消息队列都有一个关联的 msqid_ds 数据结构，其形式如下





#endif