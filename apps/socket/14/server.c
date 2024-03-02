#include "common.h"

#define MAX_BUFF_LEN (1024)

//子进程处理
void read_childproc(int sig)
{
    pid_t pid;
    int status;
    /**
     * @brief 等待特定或者指定的子进程
     * @param
     *  -1 表示等待任何子进程
     *  status 存储终止进程的状态信息
     *  WNOHANG 使waitpid 函数变味非阻塞。如果没有终止的子进程，会立即返回 0，而
     *      不会组阻塞程序的执行
    */
    pid=waitpid(-1, &status, WNOHANG);  
    LOG_INFO("remove proc id: %d", pid);
}

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server port");

    int ret, str_len = 0;
    int server_socket = 0;
    int client_socket = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    char buf[MAX_BUFF_LEN] = {0};
    pid_t child_pid;

    //设置信号
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);    //SIGCHLD 子进程终止

    //1. 构建套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_socket == 0, "socket failed");

    //2. 设置地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    //3. 绑定地址
    ret = bind(server_socket, (struct sockaddr *)&server_addr,
        sizeof(server_addr));
    CHECK_RET(ret == -1, "bind failed");

    //4. 开始监听
    ret = listen(server_socket, 5);
    CHECK_RET(ret == -1, "listen failed");
    
    while(1)
    {
        //5. accept
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
            &client_addr_len);
        if(client_socket == -1)
        {
            continue;
        }

        //6. 构建子进程进行处理
        child_pid = fork();
        CHECK_RET_GOTO(child_pid < 0, end, "fork failed");
        if(child_pid == 0)  //子进程
        {
            close(server_socket);   //子进程会复制父进程内存，所以这里需要关闭
            while((str_len = read(client_socket, buf, MAX_BUFF_LEN)) != 0)
                write(client_socket, buf, str_len);
            
            close(client_socket);
            sleep(5);
            LOG_INFO("client disconnected");
            return 0;   //子进程退出
        }
        else
        {
            close(client_socket);   //父进程关闭子进程
        }
    }
end:
    close(server_socket);
    return 0;
}