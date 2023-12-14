#include "ethernet.h"
#include "icmpv4.h"
#include "ip.h"
#include "utils.h"

void icmpv4_incoming(struct sk_buff *skb)
{
    struct iphdr *iphdr = ip_hdr(skb);
    //__((packed))__ 直接转换
    struct icmp_v4 *icmp = (struct icmp_v4 *) iphdr->data;
}