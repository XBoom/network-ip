#include "common.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    int server_sock;    //服务端套接字句柄
    int client_sock;    //就绪套接字句柄

    struct sockaddr_in server_addr;  //服务端地址
    struct sockaddr_in client_addr; //客户端地址
    socklen_t client_addr_size; //客户端地址大小

    char message[] = "hello world"; //发送的消息

    if(argc != 2)   //进程名称自己也是参数
    {
        LOG_ERROR("Usage: %s <port>", argv[0]); //端口
        exit(1);    //会终止整个程序的执行，包括所有线程
    }

    //PF_INET 与 AF_INET 都可以表达 IPV4
    //SOCK_STREAM TCP
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(server_sock == -1, "create socket failed");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    //网络字节序转换
    server_addr.sin_port = htons(atoi(argv[1]));

    ret = bind(server_sock, (struct sockaddr*)&server_addr,sizeof(server_addr));
    CHECK_RET(ret == -1,"bind server socket failed");
    
    ret = listen(server_sock, 5 );
    CHECK_RET(ret == -1, "listen server socket failed");

    //监听并回复
    client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, 
        (struct sockaddr*)&client_addr, 
        &client_addr_size);
    CHECK_RET(client_sock == -1, "accept failed");

    write(client_sock, message, sizeof(message));
    close(client_sock);
    close(server_sock);
    return 0;
}