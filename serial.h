#ifndef __SERIAL_H__
#define __SERIAL_H__

#include<reg51.h>
void serial_init();
void tx_ch(unsigned char);
void tx_mes(unsigned char*);
unsigned char rx_ch();

#endif