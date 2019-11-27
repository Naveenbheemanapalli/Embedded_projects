
#ifndef __HEADER_H__
#define __HEADER_H__

#include<reg51.h>
#define ldata P2
typedef unsigned char u8;
typedef unsigned int u16;
typedef signed int s16;
typedef signed char s8;
sbit rs = P3^4;
sbit rw = P3^5;
sbit en = P3^6;
sbit busy = P1^7;
void lcd_init();
void lcd_ready();
void lcd_cmd(u8);
void lcd_data(u8);
void mes_display(u8 s[]);
void delay_ms(u16);
#endif