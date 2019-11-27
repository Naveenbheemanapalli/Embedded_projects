#include<reg51.h>
#include"serial.h"
void serial_init(){
	SCON = 0x50;
	TMOD = 0x20;
	TH1 = 0x0FD;
	TI = TR1 = 1;
}

void tx_ch(unsigned char ch){
	SBUF = ch;
	while(TI == 0);
	TI = 0;
	TR1 = 1;
}

void tx_mes(unsigned char *s){
	while(*s){
		SBUF = *s;
		while(TI == 0);
		TI = 0;
		TR1 = 1;
		s++;
	}
	return ;
}
unsigned char rx_ch(){
	unsigned char ch;
	while(RI == 0);
	ch = SBUF;
	RI = 0;
	return ch;
}