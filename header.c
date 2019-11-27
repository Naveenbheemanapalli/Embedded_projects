#include<reg51.h>
#include"header.h"
void lcd_init(){
	lcd_cmd(0x02);
	lcd_cmd(0x02);
	lcd_cmd(0x38);
	lcd_cmd(0x0E);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
}

void delay_ms(u16 ms){
   u16 i,j;
   for(i=0;i<ms;i++)
   		for(j=0;j<1275;j++);
}
//void lcd_ready(){
//	busy = 1;
//	rs = 0;
//	rw = 1;
//	while(busy == 1){
//		en =0;
//		delay_ms(1);
//	   en =1;
//	}
//	return ;
//}
void lcd_cmd(u8 value){
	//lcd_ready();
	ldata = value;
	rs = 0;
	rw = 0;
	en = 1;
	delay_ms(1);
	en = 0;
return ;
}

void lcd_data(u8 value){
	//lcd_ready();
	ldata = value;
	rs = 1;
	rw = 0;
	en = 1;
	delay_ms(1);
	en = 0;
return ;
}

void mes_display(u8 *s){
	u8 i;
	for(i=0;s[i]!='\0';i++){
		if(i==16)
			lcd_cmd(0xc0);
		lcd_data(s[i]);
		delay_ms(1);	
	
	}
}