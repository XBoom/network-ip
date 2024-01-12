#include <netdb.h>

/**
*   @brief 通过传递字符串格式的域名获取IP地址
*   @return 成功时返回hostent结构体地址，失败时返回 NULL指针
*/
struct hostent *gethostname(const char* hostname);

struct hostent
{
    char * h_name;  //官网域名，代表着某一个主页(可能很多注明的域名并未注册官方域名)
    char ** h_aliases;  //通过多个域名访问同一个主页。同一个IP可以绑定多个域名，所以除了官方域名还可以指定其他域名
    int h_addrtype; //地址类型 IPv4则是AF_INET
    int h_length;   //保存IP地址长度(单位B)。若是IPv4地址，则是4; IPv6 则是16
    char ** h_addr_list;    //以证书形式保存域名对应的IP地址，可能多个IP分配给同一个域名
}

/**
* 利用IP地址获取域名
* addr 含有IP地址信息的in_addr结构体指针。为了同时传递IPv4地址之外的其他细心，该变量类型声明为char指针
* len 第一个参数的字节数，IPv4时为4，IPv6 时为16
* family 传递协议族信息，IPv4时为AF_INET，IPv6 时为AF_INET6
* 成功返回hostent结构体变量地址值，失败返回NULL指针
*/
struct hostent *gethostbyaddr(const char *addr, socklen_t len, int family);
