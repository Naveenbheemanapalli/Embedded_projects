#include<reg51.h>
#include"header.h"
#include"serial.h"

void main(void){
	lcd_init();
	serial_init();
	tx_ch('a');
	lcd_cmd(0x01);
	mes_display("THE RAINFALL DETECTION SYSTEM");
}