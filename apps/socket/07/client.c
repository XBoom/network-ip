


#include <sys/socket.h>

/**
 * @brief 使用UDP套接字进行发送 
 * @param 
 *      sock 用于传输数据的UDP套接字文件描述符
 *      buff 保存待传输数据的缓冲地址值
 *      nbytes 待传输的数据长度，以B为单位
 *      flags 可选参数、没有则为0
 *      to 存有目的地址信息的sockaddr解耦提变量的地址值
 *      addrlen 参数to的地址值结构体变量长度
 * @return 成功返回传输的字节数，失败时返回-1
*/
ssize_t sendto(int sock, void *buff, size_t nbytes, int flags,
                    struct sockaddr *to, socklen_t addrlen);


/// @brief 使用UDP套接字接收数据
/// @param sock 用于接收数据的UDP套接字文件描述符
/// @param buff 保存接收数据的缓冲地址值
/// @param nbytes   可接收的最大字节数，故无法超过参数buff所指的缓冲大小 
/// @param flags 可选项参数，若没有则传入0
/// @param from  存有发送端地址信息的sockaddr结构体变量的地址值
/// @param addrlen 保存参数from的结构体变量长度的变量值
/// @return 
ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags,
                    struct sockaddr *from, socklen_t *addrlen);