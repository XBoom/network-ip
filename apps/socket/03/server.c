#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server <port>");
    int ret = 0;

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;     //监听到的套接字地址大小

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(server_sock, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "bind failed");

    ret = listen(server_sock, 5);
    CHECK_RET(ret == -1, "listne failed");

    client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    CHECK_RET(client_sock == -1, "accept failed");

    int read_len = 0;
    int str_len = 0;
    char message[MAX_BUFF_LEN] = {0};
    while((read_len = read(client_sock, &message[str_len], 1)) > 0)   //每次只读一个
    {
        if(message[str_len] == '\0') //结束符(防止打印的时候被截断)
            continue;
        str_len += read_len; 
    }
    message[str_len] = '\0';
    LOG_INFO("server recv %d len msssage:\n%s", str_len, message);
    return 0;
}