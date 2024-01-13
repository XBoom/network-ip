#include "common.h"

int main(int argc, char* argv[])
{
    int sock;
    int ret = 0;
    struct sockaddr_in server_addr;
    char message[30];
    int str_len;

    CHECK_RET(argc != 3, "Usage: %s <ip> <port>", argv[0]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET_GOTO(sock == -1, end, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET_GOTO(ret, end, "connect failed");

    str_len = read(sock, message, sizeof(message));
    CHECK_RET_GOTO(str_len == -1, end, "read failed");

    LOG_INFO("message from server: %s", message);
end:
    close(sock);
    return 0;
}