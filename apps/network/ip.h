#ifndef __IP_H__
#define __IP_H__

#include "syshead.h"
#include "skbuff.h"
#include "skbuff.h"
#include "sock.h"

#define IPV4 0x04   //IPV4 协议
#define IP_TCP 0x06 //TCP协议
#define ICMPV4 0x01 //ICMP 协议
/*
    IP层数据
*/
//https://cloud.tencent.com/developer/article/1611464
struct ip_hdr 
{
    uint8_t version : 4;    //Verions(4)
    uint8_t ihl : 4;        //Header Length(4)
    uint8_t tos;            //Type of Service(8)
    uint16_t len;           //Total Length(16), 整个IP头部长度，不包括可选字段和数据部分
    uint16_t id;            //Identitfier(16)
    uint16_t frag : 3;          //Flags(3)
    uint16_t frag_offset : 13;  //Fragmented Offset(13)
    uint8_t ttl;            //Time to Live(8)
    uint8_t proto;          //Protocol(8)
    uint16_t csum;          //Header CheckSum(16)
    uint32_t saddr;         //Source Address(32)
    uint32_t daddr;         //Distination Address(32)
    uint8_t data[];         
}__attribete__((packed));

//获取IP头部
static inline struct ip_hdr* ip_hdr_init(struct sk_buff *skb)
{
    return (struct ip_hdr*)(skb->head + ETH_HDR_LEN);
}

static inline uint32_t ip_parse(char *addr)
{
    uint32_t dst = 0;
    
    if((inet_pton(AF_INET, addr, &dst) != 1))
    {
        perror("Parsing inet address failed");
        exit(1);
    }

    return ntohl(dst);
}

int ip_incoming(struct sk_buff *skb);
//输出
//int ip_output(struct sock *sk, struct sk_buff *skb);

#endif