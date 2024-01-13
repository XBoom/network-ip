#include "common.h"

#define MAX_BUF_LEN (1024)
#define CLIENT_NUM (5)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage:server <port>");

    int ret = 0;
    int read_len = 0;

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    //1. 构建套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    //2. 设置地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    //3. 绑定地址
    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET_GOTO(ret == -1, out, "bind failed");

    //4. 开始监听
    ret = listen(server_sock, 5);
    CHECK_RET_GOTO(ret == -1, out, "listen failed");

    char message[MAX_BUF_LEN] = {0};
    for(int i = 0; i < CLIENT_NUM; i++)
    {
        //5. 接收连接
        client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        CHECK_RET_GOTO(client_sock == -1, out, "accept failed");
        LOG_INFO("accept socket %d", client_sock);

        //6. 读取信息
        while((read_len = read(client_sock, message, MAX_BUF_LEN)) > 0)
        {
            LOG_INFO("recv message:%s", message);

            //7. 返回消息
            ret = write(client_sock, message, strlen(message));
            CHECK_RET_GOTO(ret == -1, out, "write failed");

            LOG_INFO("reponse len %d message:%s", strlen(message), message);
        }
        close(client_sock);
        memset(&client_addr, 0, sizeof(client_addr));
    }
    LOG_INFO("reponse message end");

out:    
    close(server_sock);
    close(client_sock);
    return 0;
}