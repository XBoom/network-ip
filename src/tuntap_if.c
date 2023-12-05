// 虚拟设备接口

#include "syshead.h"
#include "utils.h"
#include "basic.h"

static int tun_fd;  //虚拟设备句柄
static char* dev;   //设备名称
const char *tapaddr = "10.0.0.5";       //地址
const char *taproute = "10.0.0.0/24";   //路由

//设置路由
static int set_if_route(char *dev, const char *cidr)
{
    //任何发送到目标网络范围内的数据包都将通过 tap0 设备进行传输
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

//设置地址
static int set_if_address(char *dev, const char *cidr)
{
    return run_cmd("ip address add dev %s local %s", dev, cidr);
}

//设置接口为启用状态
static int set_if_up(char *dev)
{
    return run_cmd("ip link set dev %s up", dev);
}

//分配一个虚拟网络设备
// linux/Documentation/networking/tuntap.rst
static int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;
    print_error("tun_alloc: %s \n", dev);
    if ((fd = open("/dev/net/tap", O_RDWR)) < 0)
    {
        print_error("Cannot open TAP dev");
        exit(1);
    }

    CLEAR(ifr);
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);   //最大长度 16 IFNAMSIZ
        print_error("ifr name :%s \n", ifr.ifr_name);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        print_error("count not ioctl tun: %s \n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_read(char *buf, int len)
{
    return read(tun_fd, buf, len);
}

int tun_write(char *buf, int len)
{
    return write(tun_fd, buf, len);
}

//初始化虚拟设备接口
void tun_init()
{
    dev = calloc(10, 1);
    tun_fd = tun_alloc(dev);

    if(set_if_up(dev) != 0)
    {
        print_error("set up if error\n");
    }

    if (set_if_route(dev, taproute) != 0) {
        print_error("ERROR when setting route for if\n");
    }

    if (set_if_address(dev, tapaddr) != 0) {
        print_error("ERROR when setting addr for if\n");
    }
}

void free_tun()
{
    free(dev);
}