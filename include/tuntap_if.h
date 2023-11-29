#ifndef TUNTAP_H
#define TUNTAP_H

void tun_init(char *dev);
int tun_read(char *buf, int len);
int tun_write(char *buf, int len);
#endif