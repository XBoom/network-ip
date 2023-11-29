#ifndef __ICMPV4_H__
#define __ICMPV4_H__

// https://datatracker.ietf.org/doc/html/rfc792
/**
 *  
    Destination Unreachable Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                             unused                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
*/
#include "syshead.h"
#include "skbuff.h"

//Type
#define ICMP_V4_REPLY 0x00              // ICMPv4 回复消息
#define ICMP_V4_DST_UNREACHABLE 0x03    // ICMPv4 目标不可达消息
#define ICMP_V4_SRC_QUENCH      0x04    // ICMPv4 源端抑制消息
#define ICMP_V4_REDIRECT        0x05    // ICMPv4 重定向消息
#define ICMP_V4_ECHO            0x08    // ICMPv4 回显请求消息
#define ICMP_V4_ROUTER_ADV      0x09    // ICMPv4 路由器通告
#define ICMP_V4_ROUTER_SOL      0x0a    // ICMPv4 路由器请求
#define ICMP_V4_TIMEOUT         0x0b    // ICMPv4 超时消息
#define ICMP_V4_MALFORMED       0x0c    // ICMPv4 错误格式消息

//Code
//  0 = net unreachable;
//  1 = host unreachable;
//  2 = protocol unreachable;
//  3 = port unreachable;
//  4 = fragmentation needed and DF set;
//  5 = source route failed.

//Checksum 校验和

struct icmp_ipv4{
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
}__attribute__((packed));

#endif