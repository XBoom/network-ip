#include <sys/socket.h>

/**
 * @brief 半关闭
 * @param
 *  sock 要被关闭的套接字
 *  howto 传递断开的方式
 *      SHUT_RD: 断开输入流
 *      SHUT_WR: 断开输出流
 *      SHUT_RDWR: 同时断开输入输出流
 * @return 成功返回 0；失败返回 -1 
*/
int shutdown(int sock, int howto);