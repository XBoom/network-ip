#include <common.h>

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int sock = 0;   //套接字
    struct sockaddr_in server_addr;   //服务端地址

    sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    ret = connect(sock, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET(ret == -1, "connect failed");

    for(int i = 0; i < 3; i++)  //一共发送三次
    {
        //char * message = "client message \n";
        char message[] = "client message \n";
        ret = write(sock, message, sizeof(message));
        CHECK_RET(ret == -1, "write failed");
    }

    return 0;
}