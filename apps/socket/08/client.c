#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    ret = connect(server_sock, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "connect failed");

    while(1)
    {
        char message[MAX_BUFF_LEN] = {0};
        fgets(message, MAX_BUFF_LEN, stdin);
        message[strcspn(message, "\n")] = '\0';
        
        ret = write(server_sock, message, sizeof(message));
        CHECK_RET_GOTO(ret <= 0, out, "write failed");

        memset(message, 0, sizeof(message));
        ret = read(server_sock, message, MAX_BUFF_LEN);
        CHECK_RET_GOTO(ret <= 0, out, "read failed");
        LOG_INFO("recv server response:%s", message);

        memset(message, 0, sizeof(message));
    }
out:
    close(server_sock);
    return 0;
}