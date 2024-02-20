#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == 0, "socket failed");

    memset(server_addr, 0, sizeof(server_addr));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));

    ret = connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret, "connect failed"):
    char message[MAX_BUFF_LEN] = {0};
    ret = write(server_sock, (void *)message, sizeof(message));
    CHECK_RET_GOTO(ret, end, "write failed");


    ret = read(server_sock, message, sizeof(message))
    CHECK_RET_GOTO(ret, end, "read failed");

    LOG_INFO("reav %s", message);
    
end:
    close(server_sock);
    return 0;

}