#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage:server <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret != 0, "bind failed");

    char message[MAX_BUFF_LEN] = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    while(1)
    {
        ret = recvfrom(server_sock, message, MAX_BUFF_LEN, 0, 
            (struct sockaddr *)&client_addr, &client_addr_len);
        CHECK_RET_GOTO(ret < 0, out, "recv from failed");

        LOG_INFO("recv message %s from %s", message, inet_ntoa(client_addr.sin_addr));

        ret = sendto(server_sock, message, sizeof(message), 0, 
            (struct sockaddr *)&client_addr, sizeof(client_addr));
        CHECK_RET_GOTO(ret < 0, out, "send to failed");

        memset(message, 0, sizeof(message));
    }
out:
    close(server_sock);
    return 0;
}