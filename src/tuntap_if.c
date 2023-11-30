// 虚拟设备接口

#include "syshead.h"
#include "utils.h"
#include "basic.h"

static int tun_fd;

//设置路由
static int set_if_route(char *dev, char *cidr)
{
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

//设置地址
// static int set_if_address(char *dev, char *cidr)
// {
//     return run_cmd("ip route add dev %s local %s", dev, cidr);
// }

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

void tun_init(char *dev)
{
    tun_fd = tun_alloc(dev);

    if(set_if_up(dev) != 0)
    {
        print_error("set up if error\n");
    }

    if(set_if_route(dev, "10.0.0.0/24") != 0)
    {
        print_error("set route for if\n");
    }
}