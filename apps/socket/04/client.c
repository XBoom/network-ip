#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int server_sock = 0;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);   //UDP
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // ret = connect(server_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    // CHECK_RET(ret == -1, "connect failed");

    server_addr_len = sizeof(server_addr);
    for(int i = 0; i < 3; i++) 
    {
        char message[MAX_BUFF_LEN] = "client message \n";
        
        ret = sendto(server_sock, message, strlen(message) + 1, 
            0, (const struct sockaddr*)&server_addr, server_addr_len);
        CHECK_RET(ret == -1, "sendto failed");
    }

    close(server_sock);
    return 0;
}