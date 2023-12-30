#include "common.h"

#define G_SERVER_TXT_PATH ("./02/server.txt")
#define BUF_LEN (1024)

//内容写入文件
int write_file(char* path, const void *buf, ssize_t buf_len)
{
    LOG_INFO("%s %s", path, buf);
    int fd = 0;
    int ret = 0;
    fd = open(path, O_RDWR|O_CREAT);
    CHECK_RET(fd == -1, "open file %s failed", path);

    ret = write(fd, buf, buf_len);
    CHECK_RET(ret == -1, "write failed");
    
    LOG_INFO("wirte success");
    return 0;
}

int main(int argc, char* argv[])
{
    CHECK_RET_EXIT(argc != 2, "Usage: server <port>");
    int ret = 0;

    int server_sock = 0;
    int client_sock = 0;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;     //监听到的套接字地址大小

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET_GOTO(ret == -1, end, "bind failed");

    ret = listen(server_sock, 5);
    CHECK_RET_GOTO(ret == -1, end, "bind failed");

    client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct  sockaddr *)&client_addr, &client_addr_size);
    CHECK_RET_GOTO(client_sock == -1, end, "accept failed");

    char buf[BUF_LEN] = {0};
    ret = read(client_sock, buf, BUF_LEN);  //阻塞
    CHECK_RET_GOTO(ret == -1, end, "read client sock failed");

    //写入文件
    ret = write_file(G_SERVER_TXT_PATH, buf, BUF_LEN);
    if(ret)
    {
        LOG_ERROR("wirte file failed %d", ret);
        char reponse[BUF_LEN] = "write file failed";
        write(client_sock, reponse, BUF_LEN);
    }
    else 
    {
        LOG_INFO("write file success");
        char reponse[BUF_LEN] = "write file success";
        write(client_sock, reponse, BUF_LEN);
    }
end:
    close(client_sock); //重复关闭或者关闭没有打开也没有问题
    close(server_sock);
    return 0;
}