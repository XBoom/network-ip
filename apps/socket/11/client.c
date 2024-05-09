#include "common.h"

int main(int argc, char *argv[])
{
    CHECK_RET(argc != 1, "Usage: client");

    int ret = 0;
    int sock_type = 0;
    int tcp_sock = 0;
    int udp_sock = 0;
    socklen_t optlen = 0;
    optlen = sizeof(sock_type);

    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_RET(tcp_sock == -1, "socket tcp failed");

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_RET(udp_sock == -1, "socket udp failed");


    ret = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    CHECK_RET(ret == -1, "getsockopt tcp failed");
    LOG_INFO("get tcp[%d] sock opt :%d %d", SOCK_STREAM, sock_type, optlen);

    ret = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    CHECK_RET(ret == -1, "getsockopt udp failed");
    LOG_INFO("get udp[%d] sock opt :%d %d", SOCK_DGRAM, sock_type, optlen);

    //查看缓冲区大小
    int snd_buf, rcv_buf;
    ret = getsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &optlen);
    CHECK_RET(ret != 0, "get sock opt snd buf");
    LOG_INFO("tcp send buff size:%d", snd_buf);
    ret = getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &optlen);
    CHECK_RET(ret != 0, "get sock opt rcv buf");
    LOG_INFO("tcp recv buff size:%d", rcv_buf);
    ret = getsockopt(udp_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &optlen);
    CHECK_RET(ret != 0, "get sock opt snd buf");
    LOG_INFO("udp send buff size:%d", snd_buf);
    ret = getsockopt(udp_sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &optlen);
    CHECK_RET(ret != 0, "get sock opt rcv buf");
    LOG_INFO("udp recv buff size:%d", rcv_buf);

    //端口重用
    int option = 1;
    optlen = sizeof(option);
    ret = setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, 
        (void *)&option, optlen);
    CHECK_RET(ret != 0, "set tcp sockopt failed");

    ret = setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR,
        (void *)&option, optlen);
    CHECK_RET(ret != 0, "set udp sockopt failed");
    LOG_INFO("set success");
    
    return 0;
}