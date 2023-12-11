#ifndef APR_H
#define ARP_H
#include "syshead.h"
#include "ethernet.h"
#include "netdev.h"

#define ARP_ETHERNET 0x0001
#define ARP_IPV4 0x0800
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

//ARP 头部
struct arp_hdr
{
    uint16_t hwtype;    //硬件类型个，指定网络硬件的类型，比如以太网(Ethernet)
    uint16_t protype;   //使用 ARP 协议的网络层协议，例如 IPv4
    unsigned char hwsize;   //硬件地址的长度
    unsigned char prosize;  //协议地址的长度
    uint16_t opcode;        //指定ARP请求或ARP响应等操作的类型
    unsigned char data[];   
}__attribute__((packed));

// linux/include/uapi/linux/if_arp.h
// struct arphdr {
// 	__be16		ar_hrd;		/* format of hardware address	*/
// 	__be16		ar_pro;		/* format of protocol address	*/
// 	unsigned char	ar_hln;		/* length of hardware address	*/
// 	unsigned char	ar_pln;		/* length of protocol address	*/
// 	__be16		ar_op;		/* ARP opcode (command)		*/

// #if 0
// 	 /*
// 	  *	 Ethernet looks like this : This bit is variable sized however...
// 	  */
// 	unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address	*/
// 	unsigned char		ar_sip[4];		/* sender IP address		*/
// 	unsigned char		ar_tha[ETH_ALEN];	/* target hardware address	*/
// 	unsigned char		ar_tip[4];		/* target IP address		*/
// #endif

// };

//ARP 负载
struct arp_ipv4
{
    unsigned char smac[6];  //表示ARP请求或响应的发送者的硬件地址
    uint32_t sip;           //表示ARP请求或响应的发送者的网络层地址
    unsigned char dmac[6];  //通常在ARP请求中为空，表示ARP响应的目标硬件地址
    uint32_t dip;           //表示ARP请求或响应的目标网络层地址
}__attribute__((packed));

void show_arp_ipv4(struct arp_ipv4 * arp_d)
{
    printf("sip:%s, smac:%u, dip:%s, dmac:%u \n", 
        arp_d->sip, arp_d->smac, arp_d->dip, arp_d->dmac);
}

//ARP缓存
typedef struct arp_cache_entry
{
    uint16_t hwtype;
    uint32_t sip;
    unsigned char smac[6];
    unsigned int state;
}arp_cache_entry;

//ETH_HDR + ARP_HDR + data
static inline struct arp_hdr *arp_hdr_init(struct sk_buff *skb)
{
    return (struct arp_hdr *)(skb->head + ETH_HDR_LEN);
}

void arp_init();
void arp_incoming(struct netdev *netdev, struct eth_hdr *hdr);
void arp_reply(struct netdev *netdev, struct eth_hdr *hdr, struct arp_hdr *arphdr);


#endif