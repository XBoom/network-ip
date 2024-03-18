#include "fifo.h"

#define CLIENT_FIFO_CLIENT_PATH "./client/%d/send"

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;
    fifo_msg req;
    fifo_msg resp;

    ret = mkfifo(CLIENT_FIFO_CLIENT_PATH, USR_WRITE); // 写权限
    CHECK_RET(ret != 0, "mkfifo failed");

    int child_pid = fork();
    CHECK_RET(child_pid < 0, "fork failed");
    if (child_pid == 0)
    {
        server_fd = open(SERVER_FIFO_PATH_NAME, O_RDONLY);
        char buff[MSG_MAX_LEN] = {0};
        while (1)
        {
            ret = read(server_fd, buff, MSG_MAX_LEN);
            if (ret < 0)
            {
                break;
            }
            LOG_INFO("child recv %s", buff);
        }
        close(server_fd);
        LOG_INFO("child exit");
        return 0;
    }
    client_fd = open(CLIENT_FIFO_CLIENT_PATH, O_WRONLY);
    ret = send(client_fd, "hello", MSG_MAX_LEN);
    waitpid(-1, &status, WNOHANG);
    LOG_INFO("parent exit");
    return 0;
}