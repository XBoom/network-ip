#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 3, "Usage: client <ip> <port>");

    int ret = 0;
    int client_fd = 0;
    int server_sock = 0;
    int send_sock = 0;
    struct sockaddr_in server_addr;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    send_sock = connect(server_sock, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET(send_sock == -1, "connect failed");

    client_fd = open("client.txt", O_RDWR|O_CREAT);
    CHECK_RET_GOTO(client_fd == -1, out, "open failed");

    int read_len = 0;
    char message[MAX_BUFF_LEN] = {0};
    while((read_len = read(send_sock, message, MAX_BUFF_LEN)))
    {
        ret = write(client_fd, message, sizeof(message));
        CHECK_RET_GOTO(ret == -1, out, "fwrite failed");
    }
    char reponse[MAX_BUFF_LEN] = "recv all file";
    ret = write(send_sock,reponse, sizeof(reponse));
    CHECK_RET_GOTO(ret == -1, "write failed");
    
out:
    close(client_fd);
    close(server_sock);
    close(send_sock);
    return 0;
}