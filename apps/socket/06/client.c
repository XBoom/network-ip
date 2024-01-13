#include "common.h"

#define MAX_BUFF_LEN (1024)

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
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    //3. 发起连接
    ret = connect(client_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "connect failed");

    while(1)    
    {
        char message[MAX_BUFF_LEN] = {0};
        fgets(message, MAX_BUFF_LEN, stdin);
        CHECK_RET(strcmp(message, "Q\n") == 0, "exit normal");

        LOG_INFO("input message:%s", message);
        ret = write(client_sock, message, strlen(message) - 1);
        CHECK_RET(ret == -1, "write failed");

        char reponse[MAX_BUFF_LEN] = {0};
        ret = read(client_sock, &reponse, MAX_BUFF_LEN);
        CHECK_RET(ret == -1, "read failed");
        LOG_INFO("reponse message:%s", reponse);

        memset(&message, 0, sizeof(message));
        memset(&reponse, 0, sizeof(reponse));
    }
    close(client_sock);
    return 0;
}