#include "common.h"

#define MAX_BUFF_LEN (1024)

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 2, "Usage: server port");

    int ret = 0;
    int server_socket = 0;
    int client_socket = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //1. 创建套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_socket == -1, "socket failed");

    //2. 注册一个信号
    

    //2. 设置地址
    memset(server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    //3. 绑定地址
    

    //4. listen
    
    while(1)
    {
        //5. accept
        client_socket = accept();
        CHECK_RET(client_socket == -1, "accept failed");

        //6. 构建子进程进行处理
        pid_t child_pid = fork();
        CHECK_RET_GOTO(child_pid < 0, end, "fork failed");

        if(child_pid == 0)  //子进程
        {

        }

    }

end:

    return 0;
}