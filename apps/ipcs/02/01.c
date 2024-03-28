#include "fifo.h"
#include "request.pb-c.h"

#define CLIENT_FIFO_CLIENT_PATH "./client/%d/send"

// 显示响应消息
void show_response_message(FifoResponse *resp)
{
    FifoRespHead *head = resp->head;

    LOG_INFO("head: %d %d %d", head->id, head->code, head->code);
    LOG_INFO("body: %s", resp->body);
}

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;
    fifo_msg req;
    fifo_msg resp;

    pid_t pid = 0;
    // 1. 根据当前进程 id 设置管道路径
    char *fifo_path = sprintf(CLIENT_FIFO_CLIENT_PATH, getpid());

    // 2. 构建管道
    ret = mkfifo(fifo_path, USR_WRITE); // 写权限
    CHECK_RET(ret != 0, "mkfifo failed");

    // 3. 构建子进程
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
    // 4. 构造一个消息
    uint8_t buf[MSG_MAX_LEN];
    FifoRequest *request = FIFO_REQUEST__INIT();
    request->head->id = getpid();
    strcpy(request->head->fifo, fifo_path);
    request->head->len = 0;
    int len = fifo_request__pack_to_buffer(request, buf, sizeof(buf));

    // 5. 打开服务端监听管理，把消息写进去
    client_fd = open(SERVER_FIFO_PATH_NAME, O_WRONLY);
    CHECK_RET(client_fd < 0, "open fifo failed");

    // 6. 监听消息
    ret = write(client_fd, buf, MSG_MAX_LEN);
    waitpid(-1, &status, WNOHANG);
    LOG_INFO("parent exit");
    return 0;
}