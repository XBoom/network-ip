#ifndef __FIFO_H__
#define __FIFO_H__

#include "common.h"

#define MSG_MAX_LEN (1024)
#define MAX_FIFO_PATH_LEN (20)
#define SERVER_FIFO_PATH_NAME "./server/01/receive"

// 表示文件所有者具有读写权限，所属组和其他用户只有读权限
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
// 自己仅有读权限，而其他组和其他用户有写权限
#define USR_READ (S_IRUSR | S_IWGRP | S_IWOTH)
// 自己仅有写权限，而其他组和其他用户有读权限
#define USR_WRITE (S_IWUSR | S_IRGRP | S_IROTH)

typedef struct fifo_msg
{
    uint32_t req_id;
    uint32_t body_len;
    char fifo_name[MAX_FIFO_PATH_LEN];
    char body[];
} fifo_msg;

#endif