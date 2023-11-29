#include "ethernet.h"
#include "icmpv4.h"
#include "ip.h"
#include "utils.h"

//处理  ICMP_V4_ECHO
void icmpv4_reply(struct sk_buff *skb)
{
    struct ip_hdr *ip_h = ip_hdr_init(skb);
    struct icmp_ipv4 *icmp;

    //
    
}

void icmpv4_incoming(struct sk_buff *skb)
{
    struct iphdr *iphdr = ip_hdr(skb);
    //__((packed))__ 直接转换
    struct icmp_v4 *icmp = (struct icmp_v4 *) iphdr->data;

    switch(icmp->type)
    {
        case:ICMP_V4_ECHO:
            icmpv4_reply(skb);
        break;
        default:
            print_error("not support icmp type %" PRIu8 "\n", icmp->type);
            goto drop_skb;
    }
drop_skb:
    free_skb(skb);
    skb = NULL;
}