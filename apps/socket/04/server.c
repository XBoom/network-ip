#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clietn_addr_len = 0;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(server_sock, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "bind failed");

    int str_len = 0;
    int read_len = 0;
    char message[MAX_BUFF_LEN] = {0};
    while((read_len = recvfrom(server_sock, &message[str_len], 1, 0,
        (struct sockaddr *)&client_addr, &clietn_addr_len)) > 0)
    {
        LOG_INFO("recv %d %d", str_len, read_len);
        str_len += read_len;
    }
    LOG_INFO("read udp client %d len message: %s", message);
}