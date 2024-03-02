#include "common.h"

#define MAX_BUFF_SIZE (30)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;

    //1. 构建套件字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == 0, "sock failed");

    //2. 设置地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    //3. 连接
    ret = connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret != 0, "connect failed");

    //4. 发送数据
    char message[MAX_BUFF_SIZE] = "hello world";
    ret = write(server_sock, message, sizeof(message));
    CHECK_RET(ret == 0, "write failed");

    //5. 接收数据
    char buff[MAX_BUFF_SIZE] = {0};
    ret = read(server_sock, buff, MAX_BUFF_SIZE);
    CHECK_RET(ret == 0, "read failed");

    LOG_INFO("recv %s", buff);

    return 0;
}