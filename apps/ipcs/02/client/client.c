#include "fifo.h"
#include "request.pb-c.h"

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;
    char cli_fifo_path[MSG_MAX_LEN] = {0};

    sprintf(cli_fifo_path, FIFO_CLIENT_PATH_FMT,
            CLIENT_FIFO_PATH_NAME, 0);

    // 1. 构建管道(服务端管道)
    ret = mkfifo(cli_fifo_path, FILE_MODE);
    if (ret < 0)
    {
        LOG_ERROR("mkfifo failed: %s", strerror(errno));
        return -1;
    }

    // 2. 构建子进程
    int child_pid = fork();
    CHECK_RET(child_pid < 0, "fork failed");
    if (child_pid == 0)
    {
        // 1. 打开客户端fifo路径
        server_fd = open(cli_fifo_path, O_RDONLY);
        if (!server_fd)
        {
            LOG_ERROR("open cli fifo path %s failed", cli_fifo_path);
            return -1;
        }
        uint8_t buff[MSG_MAX_LEN] = {0};

        ret = read(server_fd, buff, MSG_MAX_LEN);
        if (ret < 0)
        {
            goto child_end;
        }
        // 构造接收消息结构
        FifoResponse *resp = fifo_response__unpack(NULL, ret, buff);
        if (!resp)
        {
            LOG_ERROR("child recv null");
            close(server_fd);
            goto child_end;
        }
        LOG_INFO("child recv %s", resp->body);

    child_end:
        if (resp)
            fifo_response__free_unpacked(resp, NULL); // 释放消息
        close(server_fd);
        LOG_INFO("child exit");
        return 0;
    }
    else
    {
        // 4. 构造一个消息
        FifoRequest request = FIFO_REQUEST__INIT;
        request.fifo = (char *)malloc(sizeof(cli_fifo_path));
        memcpy(request.fifo, cli_fifo_path, sizeof(cli_fifo_path));

        size_t packed_size = fifo_request__get_packed_size((const FifoRequest *)&request);
        uint8_t *packed_data = NULL;
        packed_data = (uint8_t *)malloc(packed_size);
        fifo_request__pack(&request, packed_data);

        // 5. 打开服务端监听管理，把消息写进去
        client_fd = open(SERVER_FIFO_PATH_NAME, O_WRONLY);
        CHECK_RET(client_fd < 0, "open fifo failed");
        LOG_INFO("send fifo :%s %d", cli_fifo_path, packed_size);
        // 6. 发送消息
        ret = write(client_fd, packed_data, packed_size);
        CHECK_RET_GOTO(ret < 0, end, "write failed");
    end:
        close(client_fd);
        waitpid(-1, &status, 0);
        unlink(cli_fifo_path);
        LOG_INFO("parent exit");
        return 0;
    }
}