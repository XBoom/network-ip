#include "ethernet.h"
#include "icmpv4.h"
#include "ip.h"
#include "utils.h"

//处理  ICMP_V4_ECHO
void icmp_v4_reply(struct sk_buff *skb)
{
    struct ip_hdr *ip_h = ip_hdr_init(skb);
    struct icmp_v4 *icmp;

    //
    ip_ou
    
}

void icmpv4_incoming(struct sk_buff *skb)
{
    struct ip_hdr *iphdr = ip_hdr_init(skb);
    //__((packed))__ 直接转换
    struct icmp_v4 *icmp = (struct icmp_v4 *) iphdr->data;

    switch(icmp->type)
    {
        case:ICMP_V4_ECHO:
            icmp_v4_reply(skb);
            return;
        case:ICMP_V4_DST_UNREACHABLE:
            LOG_ERROR("ICMPv4 received 'dst unreachable' code %d, "
                  "check your routes and firewall rules\n", icmp->code);
            goto drop_skb;
        default:
            LOG_ERROR("not support icmp type %" PRIu8 "\n", icmp->type);
            goto drop_skb;
    }
drop_skb:
    free_skb(skb);
    skb = NULL;
}


