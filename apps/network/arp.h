#ifndef APR_H
#define ARP_H
#include "syshead.h"
#include "ethernet.h"
#include "netdev.h"

#define ARP_ETHERNET 0x0001
#define ARP_IPV4 0x0800

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

#define ARP_OP_REQUEST 1    /**< request to resolve address */
#define ARP_OP_REPLY 2      /**< response to previous request */
#define ARP_OP_REVREQUEST 3 /**< request proto addr given hardware */
#define ARP_OP_REVREPLY 4   /**< response giving protocol address */
#define ARP_OP_INVREQUEST 8 /**< request to identify peer */
#define ARP_OP_INVREPLY 9   /**< response identifying peer */

#define ARP_HDR_LEN sizeof(struct arp_hdr)   // arp 头部大小
#define ARP_IPV4_LEN sizeof(struct arp_ipv4) // arp ipv4 数据大小

// ARP 头部
struct arp_hdr
{
    uint16_t hardware;      // 硬件类型
    uint16_t protocol;      // 上层协议类型，例如 IPv4
    unsigned char hw_len;   // MAC地址长度
    unsigned char pro_len;  // 协议地址长度
    uint16_t opcode;        // 指定ARP请求或ARP响应等操作的类型
    unsigned char data[];
} __attribute__((packed));

// ARP 负载
struct arp_ipv4
{
    unsigned char smac[6]; // 表示ARP请求或响应的发送者的硬件地址
    uint32_t sip;          // 表示ARP请求或响应的发送者的网络层地址
    unsigned char dmac[6]; // 通常在ARP请求中为空，表示ARP响应的目标硬件地址
    uint32_t dip;          // 表示ARP请求或响应的目标网络层地址
} __attribute__((packed));

// ARP缓存
typedef struct arp_cache_entry
{
    uint16_t hwtype;
    uint32_t sip;
    unsigned char smac[6];
    unsigned int state;
} arp_cache_entry;

// ETH_HDR + ARP_HDR + data
static inline struct arp_hdr *arp_hdr_init(struct sk_buff *skb)
{
    return (struct arp_hdr *)(skb->head + ETH_HDR_LEN);
}

uint32_t arp_init();
void arp_reply(struct sk_buff *skb, struct netdev *netdev);
void arp_receive(struct sk_buff *skb);
void show_arp_ipv4(struct arp_ipv4 *arp_d);

#endif