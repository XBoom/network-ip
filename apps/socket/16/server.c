#include "common.h"

#define MAX_BUFF_SIZE (30)

void child_proc(int sig) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        LOG_INFO("Child %d exited", pid);
    }

    if (pid == -1 && errno != ECHILD) {
        LOG_ERROR("waitpid failed");
    }
}

void cleanup(int file_pipe[2]) {
    close(file_pipe[0]);
    close(file_pipe[1]);
}

int main(int argc, char *argv[]) {
    CHECK_RET(argc != 2, "Usage: server <port>");

    int ret = 0;
    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int str_len = 0;
    char buf[MAX_BUFF_SIZE];

    // 一个管道用来写文件
    int file_pipe[2] = {0};
    ret = pipe(file_pipe);
    CHECK_RET(ret != 0, "pipe failed");

    // 构建套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "socket failed");

    // 设置信号
    struct sigaction act;
    act.sa_handler = child_proc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;  // 使系统调用在中断后自动重启
    sigaction(SIGCHLD, &act, 0);

    // 设置地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // 绑定地址
    ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    CHECK_RET(ret != 0, "bind failed");

    // 开启监听
    ret = listen(server_sock, 5);
    CHECK_RET(ret != 0, "listen failed");

    // 构建一个子进程用来存储文件
    pid_t child_file_pid = fork();
    CHECK_RET(child_file_pid == -1, "fork failed");

    if (child_file_pid == 0) { // 子的写进程
        FILE *fp = fopen("fork_pipe.txt", "wt");
        CHECK_RET(fp == NULL, "fopen failed");

        for (int i = 0; i < 10; i++) {
            ssize_t len = read(file_pipe[0], buf, MAX_BUFF_SIZE);
            if (len > 0) {
                ret = fwrite(buf, 1, len, fp);
                LOG_INFO("fwrite %.*s %zd", (int)len, buf, len);
            }
        }

        fclose(fp);
        cleanup(file_pipe);
        return 0;
    }

    // 接收连接并读取
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        
        if (client_sock == -1) {
            LOG_INFO("accept failed");
            continue;
        }

        pid_t pid = fork();
        CHECK_RET(pid == -1, "fork failed");

        if (pid == 0) {
            close(server_sock);

            while ((str_len = read(client_sock, buf, MAX_BUFF_SIZE)) != 0) {
                write(client_sock, buf, str_len);
                write(file_pipe[1], buf, str_len);
                LOG_INFO("write %.*s success", str_len, buf);
            }

            close(client_sock);
            cleanup(file_pipe);
            return 0;
        } else {
            close(client_sock);
        }
    }

    LOG_INFO("process exit");
    cleanup(file_pipe);
    close(server_sock);
    return 0;
}