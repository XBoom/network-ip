#include "syshead.h"
#include "skbuff.h"
#include "arp.h"
#include "ip.h"
#include "icmpv4.h"
#include "tcp.h"
#include "utils.h"

//将网络字节序ip头转为本地字节序
static void ip_init_pkt(struct ip_hdr *ip_h)
{
    ip_h->saddr = ntohl(ip_h->saddr);
    ip_h->daddr = ntohl(ip_h->daddr);
    ip_h->len = ntohs(ip_h->len);
    ip_h->id = ntohs(ip_h->id);
}

int ip_recv(struct sk_buff *skb)
{
    struct ip_hdr *ip_h = ip_hdr_init(skb); //ETH_HDR_LEN

    uint16_t sum = -1;

    //为什么它不用通过网络字节序转本地字节序(因为只有一个字节)
    if(ip_h->version != IPV4)   
    {
        print_error("Data gram version was not IPV4 \n");
        goto drop_skb;
    }

    if(ip_h->ihl < 5)   //为什么不能小于5，最小值为5个32位字，也就是20字节
    {
        print_error("Time to live of datagram reached 0\n");
        goto drop_skb;
    }

    if (ih->ttl == 0) {
        //TODO: Send ICMP error
        print_err("Time to live of datagram reached 0\n");
        goto drop_pkt;
    }

    //校验和(合上面对应，ip_h->ihl 表示的是多少 32位)
    sum = checksum(ip_h, ip_h->ihl * 4, 0);
    if(sum != 0)
    {
        print_error("Invalid checksum, drop packet handling \n");
        goto drop_skb;
    }

    ip_init_pkt(ip_h);
    switch (ip_h->proto) {  //判断上层协议
    case ICMPV4:        // 0x01
        icmpv4_incoming(skb);
        return 0;
    case IP_TCP:        //0x06
        tcp_in(skb);
        return 0;
    default:
        print_err("Unknown IP header proto\n");
        goto drop_pkt;
    }

drop_skb:
    free_skb(skb);
    return 0;
}