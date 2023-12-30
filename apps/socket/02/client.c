#include "common.h"


#define G_CLIENT_TXT_PATH "./02/client.txt"
#define BUF_LEN (1024)

//读取文件内容
int read_file(char *path, int flag, char *buf, int buf_len)
{
    int fd;
    fd = open(path, flag);
    CHECK_RET(fd == -1, "open file %s failed", path);

    int n = 0;
    n = read(fd, buf, buf_len);
    CHECK_RET_GOTO(n == -1, end, "read failed %d ", n); //需要关闭句柄

end:
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_RET_EXIT(argc != 3, "Usage: client <ip> <port>");
    int ret = 0;

    int sock = 0;   //套接字句柄
    struct sockaddr_in server_addr; //服务器地址

    //读取文件内容
    char buf[BUF_LEN] = {0};
    ret = read_file(G_CLIENT_TXT_PATH, O_RDONLY, buf, BUF_LEN);
    CHECK_RET(ret, "read file failed");

    //构建套接字发送
    sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(sock == -1,"socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    CHECK_RET_GOTO(ret == -1, end, "connect failed");

    ret = write(sock, buf, BUF_LEN);
    CHECK_RET_GOTO(ret == -1, end, "write failed");
    LOG_INFO("client send msg success");

    char reponse[BUF_LEN] = {0};
    ret = read(sock, reponse, BUF_LEN); //阻塞
    CHECK_RET_GOTO(ret == -1, end, "write failed");

    LOG_INFO("receive server reponse:%s", reponse);
end:
    close(sock);
    return 0;
}