#include "fifo.h"
#include "request.pb-c.h"

#define CLIENT_FIFO_CLIENT_PATH "./client/%d/send"

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;

    // 1. 根据当前进程 id 设置管道路径
    char fifo_path[1024] = {0};
    sprintf(fifo_path, CLIENT_FIFO_CLIENT_PATH, (int)getpid());

    // 2. 构建管道
    ret = mkfifo(fifo_path, USR_WRITE); // 写权限
    CHECK_RET(ret != 0, "mkfifo failed");

    // 3. 构建子进程
    int child_pid = fork();
    CHECK_RET(child_pid < 0, "fork failed");
    if (child_pid == 0)
    {
        server_fd = open(fifo_path, O_RDONLY);
        uint8_t buff[MSG_MAX_LEN] = {0};
        while (1)
        {
            ret = read(server_fd, buff, MSG_MAX_LEN);
            if (ret < 0)
            {
                break;
            }
            // 构造接收消息结构
            FifoResponse *resp = fifo_response__unpack(NULL, MSG_MAX_LEN, buff);
            if (resp == NULL)
            {
                LOG_ERROR("child recv null");
                return -1;
            }
            LOG_INFO("child recv %s", resp->body);

            fifo_response__free_unpacked(resp, NULL);  // 释放消息
        }
        close(server_fd);
        LOG_INFO("child exit");
        return 0;
    }
    // 4. 构造一个消息
    FifoRequest request = FIFO_REQUEST__INIT;
    request.fifo = fifo_path;

    size_t packed_size;
    uint8_t *packed_data = NULL;
    fifo_request__pack_to_buffer(&request, &packed_size);
    packed_data = (uint8_t *)malloc(packed_size);
    fifo_request__pack(&request, packed_data, packet_size);

    // 5. 打开服务端监听管理，把消息写进去
    client_fd = open(SERVER_FIFO_PATH_NAME, O_WRONLY);
    CHECK_RET(client_fd < 0, "open fifo failed");

    // 6. 发送消息
    ret = write(client_fd, packed_data, packet_size);
    CHECK_RET(ret < 0, "write failed");
    waitpid(-1, &status, WNOHANG);
    LOG_INFO("parent exit");
    return 0;
}