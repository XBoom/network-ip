#include "arp.h"
#include "netdev.h"

//全局缓存
static arp_cache_entry arp_cache[ARP_CACHE_LEN];

//插入arp表
static int insert_arp_translation_table(struct arp_hdr *hdr, 
    struct arp_ipv4 *data)
{
    arp_cache_entry *entry;
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {
        entry = &arp_cache[i];

        if(entry->state == ARP_FREE)
        {
            entry->state = ARP_RESOLVED;

            entry->hwtype = hdr->hwtype;
            entry->sip = data->sip;
            memcpy(entry->smac, data->smac, sizeof(sizeof(entry->smac)));
        }
    }
}