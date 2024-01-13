#include "common.h"

#define MAX_BUFF_LEN (1024)
#define SERVER_TXT_PATH "./09/server.txt"

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server <port>");

    int ret = 0;
    int server_sock = 0;
    int client_sock = 0;
    FILE* server_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = 0;

    //构建套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    //设置服务端地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    //设置服务端套接字地址
    ret = bind(server_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "bind failed");

    //监听
    ret = listen(server_sock, 5);
    CHECK_RET(ret == -1, "listen failed");

    char buff[MAX_BUFF_LEN] = {0};
    client_addr_len = sizeof(client_addr);
    while (1)
    {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        CHECK_RET_GOTO(client_sock == -1, out, "accept failed");

        server_fd = fopen(SERVER_TXT_PATH, "r");
        CHECK_RET_GOTO(server_fd == NULL, out, "fopen failed");

        while(fgets(buff, sizeof(buff), server_fd))
        {
            ret = write(client_sock, buff, sizeof(buff));
            CHECK_RET_GOTO(ret == -1, out, "write failed");
        }
        //关闭写
        shutdown(client_sock, SHUT_WR);

        memset(buff, 0 , sizeof(buff));
        while(read(client_sock, buff, sizeof(buff)))
        {
            LOG_INFO("server recv %s", buff);
        }
        //关闭读
        shutdown(client_sock, SHUT_RD);
    }
out:
    close(server_sock);
    fclose(server_fd);
    return 0;
}