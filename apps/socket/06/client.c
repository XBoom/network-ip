#include "common.h"

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int client_sock;
    struct sockaddr_in server_addr;

    //1. 构建套接字
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(client_sock == -1, "socket failed");

    //2. 设置地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[0]);
    server_addr.sin_port = htons(atoi(argv[2]));

    //3. 发起连接
    ret = connect(client_sock, (const sturct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "connect failed");

    

    return 0;
}