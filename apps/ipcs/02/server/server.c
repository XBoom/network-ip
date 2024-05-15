#include "fifo.h"
#include "request.pb-c.h"

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;
    size_t buffer_size = MSG_MAX_LEN;

    // 1. 构建管道(服务端管道)
    ret = mkfifo(SERVER_FIFO_PATH_NAME, FILE_MODE);
    if (ret < 0)
    {
        LOG_ERROR("mkfifo failed: %s", strerror(errno));
        return -1;
    }

    // 3. 构建子进程
    int child_pid = fork();
    CHECK_RET(child_pid < 0, "fork failed");
    if (child_pid == 0)
    {
        server_fd = open(SERVER_FIFO_PATH_NAME, O_RDONLY);
        uint8_t *buff = malloc(buffer_size);
        CHECK_RET(buff == NULL, "malloc buff failed");

        while (1)
        {
            ret = read(server_fd, buff, MSG_MAX_LEN);
            if (ret <= 0)
            {
                break;
            }
            // 构造接收请求
            FifoRequest *req = fifo_request__unpack(NULL, ret, buff);
            if (!req)
            {
                LOG_ERROR("Failed to unpack message from FIFO");
                continue;
            }

            // 打开客户端告知的 fifo，并将消息发送回去
            LOG_INFO("recv client request %s %d", req->fifo, ret);
            client_fd = open(req->fifo, O_WRONLY);
            CHECK_RET(client_fd < 0, "open failed");

            fifo_request__free_unpacked(req, NULL);
            // 响应消息
            FifoResponse resp = FIFO_RESPONSE__INIT;
            resp.body = "hello world";

            size_t packed_size = fifo_response__get_packed_size((const FifoResponse *)&resp);
            uint8_t *packed_data = (uint8_t *)malloc(packed_size);
            fifo_response__pack(&resp, packed_data);

            // 6. 发送消息
            ret = write(client_fd, packed_data, packed_size);
            CHECK_RET(ret < 0, "write failed");
            LOG_INFO("write finished");
        }
        free(buff);
        close(server_fd);
        LOG_INFO("child exit");
        return 0;
    }
    else
    {
        // 监听子进程
        waitpid(child_pid, &status, 0); // WNOHANG TODO
        unlink(SERVER_FIFO_PATH_NAME);
        LOG_INFO("parent exit");
        return 0;
    }
}