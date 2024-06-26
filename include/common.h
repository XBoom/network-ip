#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>  //标准输入输出
#include <stdint.h> // uint8_t type
#include <stdlib.h> //atoi 讲字符串表示整数
#include <string.h> //字符处理 memset
#include <unistd.h> //管道
#include <errno.h>  //错误码

#include <sys/types.h> //mkfifo
#include <sys/stat.h>

#include "xsocket.h" //套接字头部
#include "xfile.h"   //文件头部

#include <sys/wait.h> //wait
#include <signal.h>   //信号

#include "lib_log.h" //自定义日志文件
#include "list.h"    //链表
#include "utils.h"   //工具类

#include "uev.h" //事件驱动

#include "times.h" //时间

#endif