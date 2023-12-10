#ifndef __ip_h__
#define __ip_h__

#define IPV4 0x04   //IPV4 协议
#define IP_TCP 0x06 //TCP协议
#define ICMPV4 0x01 //ICMP 协议
/*
    IP层数据
*/
//https://cloud.tencent.com/developer/article/1611464
typedef struct ip_hdr 
{
    uint8_t version : 4;    //Verions(4)
    uint8_t ihl : 4;        //Header Length(4)
    uint8_t tos;            //Type of Service(8)
    uint16_t len;           //Total Length(16)
    uint16_t id;            //Identitfier(16)
    uint16_t frag : 3;          //Flags(3)
    uint16_t frag_offset : 13;  //Fragmented Offset(13)
    uint8_t ttl;            //Time to Live(8)
    uint8_t proto;          //Protocol(8)
    uint16_t csum;          //Header CheckSum(16)
    uint32_t saddr;         //Source Address(32)
    uint32_t daddr;         //Distination Address(32)
    uint8_t data[];         
}__attribete__((packed)) ip_hdr;

#endif