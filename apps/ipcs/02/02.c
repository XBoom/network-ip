#include "fifo.h"
#include "request.pb-c.h"

int main(int argc, char *argv[])
{
    int client_fd = 0; // 客户端 fifo 标识符，用来接收其他服务的消息
    int server_fd = 0; // 服务端 fifo 标识符，用来向服务端发送消息
    int ret = 0;
    int status = 0;

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
            // 构造接收请求
            FifoRequest req = FIFO_REQUEST__INIT;
            fifo_request__unpack(buff, MSG_MAX_LEN, &req);

            // 打开客户端告知的 fifo，并将消息发送回去
            LOG_INFO("recv client request %s", req.fifo);
            client_fd = open(req.fifo, O_WRONLY);
            CHECK_RET(client_fd < 0, "open failed");

            // 响应消息
            FifoResponse resp = FIFO_RESPONSE__INIT;
            request->body = "hello world";

            size_t packed_size;
            uint8_t *packed_data = NULL;
            fifo_response__pack_to_buffer(&resp, &packed_size);
            packed_data = (uint8_t *)malloc(packed_size);
            fifo_request__pack(&resp, packed_data, packet_size);

            // 6. 发送消息
            ret = write(client_fd, packed_data, packet_size);
            CHECK_RET(ret < 0, "write failed");
        }
        close(server_fd);
        LOG_INFO("child exit");
        return 0;
    }

    // 监听子进程
    waitpid(-1, &status, WNOHANG);
    LOG_INFO("parent exit");
    return 0;
}