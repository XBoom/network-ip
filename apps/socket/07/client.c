#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc!= 3, "Usage:client <ip> <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    char message[MAX_BUFF_LEN] = {0};
    char response[MAX_BUFF_LEN] = {0};
    socklen_t server_addr_len = 0;
    while(1)
    {
        fgets(message, MAX_BUFF_LEN - 1, stdin);
        message[strcspn(message, "\n")] = '\0';
        LOG_INFO("client send %s to %s", message, argv[1]);
        ret = sendto(server_sock, message, MAX_BUFF_LEN - 1, 0,
            (struct sockaddr*)&server_addr, sizeof(server_addr));
        CHECK_RET_GOTO(ret < 0, out, "send to failed");
        
        ret = recvfrom(server_sock, response, MAX_BUFF_LEN - 1, 0, 
            (struct sockaddr *)&server_addr, &server_addr_len);
        CHECK_RET_GOTO(ret < 0, out, "recvfrom failed");
        LOG_INFO("client recv %s from %s", response, argv[1]);

        memset(message, 0, sizeof(message));
        memset(response, 0, sizeof(response));
    }
out:
    close(server_sock);
    return 0;
}
