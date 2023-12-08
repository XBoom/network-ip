#ifndef __TUNTAP_H__
#define __TUNTAP_H__

void tun_init();
int tun_read(char *buf, int len);
int tun_write(char *buf, int len);
void free_tun();
#endif