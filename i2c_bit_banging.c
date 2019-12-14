/********************************************************
*	I2C BIT - BANGING CODE FOR 8051(AT89S52) micro- controller for digital clock in the LCD
*	@ 12MHZ clk frequency
* 	taking micro controller as master and slave - DS1307(RTC) timer
*	Thank Q
*********************************************************/
#include<reg51.h>
#define ldata P2
void lcd_cmd(unsigned char);
void delay_ms(unsigned int itime);
typedef unsigned char u8;
typedef signed char s8;
sbit SDA = P1^0;		  	//declaring the SDA pin in port 1 pin -0
sbit SCL = P1^1;			//declaring the SCL pin in port 1 pin -1
sbit rs = P3^4;				//Lcd interfacing pins of rs/rw/ena
sbit rw = P3^5;
sbit en = P3^6;

void lcd_init(){			//LCD initilizaion
	lcd_cmd(0x02);
	lcd_cmd(0x02);
	lcd_cmd(0x38);
	lcd_cmd(0x0E);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
}

void lcd_cmd(u8 val){		//LCD command execution
	//lcd_ready();
	ldata = val;
	rs = 0;
	rw = 0;
	en = 1;
	delay_ms(1);
	en = 0;
return ;
}

void lcd_data(u8 val){		   //printing DAta on the LCD
	//lcd_ready();
	ldata = val;
	rs = 1;
	rw = 0;
	en = 1;
	delay_ms(1);
	en = 0;
return ;
}

void i2c_start(){				  //generating start_condition of i2c device
	SCL = 1;SDA=1;SDA=0;
}

void i2c_stop(){				  //generating stop_condition of i2c device
	SCL=0;SDA=0;SCL=1;SDA=1;
}

void i2c_write(u8 temp){		 //generating write_condition of i2c device to write into the memory of slave
	s8 i;
	for(i=7;i>=0;i--){
	 	SCL = 0;
		SDA = (temp&(1<<i))?1:0;
		SCL = 1;
	}
}

u8 i2c_read(){				 //generating read_condition of i2c device to read from the memory of slave
	u8 temp=0x00;
	s8 i;
	for(i=7;i>=0;i--){
		SCL =1;
		if(SDA == 1)
			temp = temp|(1<<i);
		SCL = 0;
	}
return temp;
}

void i2c_noack(){				//generating no_ack_condition of i2c device
	SCL = 0;SDA=1;SCL=1;
}
void ack_delay(){				//generating ack_condition of i2c device
	unsigned int i;
	for(i=0;i<2;i++);
}
u8 i2c_ack(){
	SCL=0;SDA=1;SCL=1;
	ack_delay();	
	if(SDA==0){
		SCL =0;
		return 0x01; }
	else{
		SCL = 0;
		return 0x00;}
}											
/************************************************************************************
* WRITE FRAME OF THE I2C PROTOCAL
*
*
* start		| write 7 bit slave address | ACK |	write 8-bit of 	     | ACK | write the 		      | ACK | stop
* condition | with write option			|	  |	slave memory address |	   | 8-bit data in memory |	    | condition
*
*
**************************************************************************************/

void device_write(u8 slave_addr,u8 mem_addr,u8 dta){
	u8 t;
	i2c_start();
	i2c_write(slave_addr);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
	i2c_write(mem_addr);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
	i2c_write(dta);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
stop_cond :
	i2c_stop();
}
/************************************************************************************
* READ FRAME OF THE I2C PROTOCAL
*
* start		| write 7 bit slave address | ACK |	write 8-bit of 	     | ACK | start    | write 7 bit slave address | ACK |read the 		        | NO | stop
* condition | with write option			|	  |	slave memory address |	   | conditon | with read option		  |     |8-bit data from memory | ACK| condition
*|																		   |
*|	DUMMY write conditon for the I2C device in order to providing		   |
*|	address from where we can read the data of slave memory				   |
*|                                                                         |
**************************************************************************************/
u8 device_read(u8 slave_addr,u8 mem_addr){
	u8 t,temp;
	i2c_start();
	i2c_write(slave_addr);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
	i2c_write(mem_addr);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
	i2c_start();
	i2c_write(slave_addr|0x01);
	t = i2c_ack();
	if(t==0)
		goto stop_cond;
	temp = i2c_read();
	i2c_noack();
stop_cond :
	i2c_stop();
return temp;
}

void delay_ms(unsigned int itime){
	unsigned int i,j;
	for(i=0;i<itime;i++)
		for(j=0;j<1275;j++);
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
void main(){
	u8 h,mm,sec,d,m,y,dow;
	lcd_init();
	device_write(0xD0,0X02,0X23);	//writing the present time to the RTC device
	device_write(0xD0,0X01,0X59);
	device_write(0xD0,0X00,0X50);
	while(1){
	lcd_cmd(0xC);lcd_cmd(0x80);
	   h = device_read(0XD0,0x02);		  //reading the hours and minutes and secoonds from RTC
	   mm = device_read(0XD0,0x01);
	   sec = device_read(0XD0,0x00);
	   d = device_read(0xD0,0x06);
	   lcd_data((h/16)+48);						  //Displaying on the LCD
	   lcd_data((h%16)+48);lcd_data(':');
	   lcd_data((mm/16)+48);
	   lcd_data((mm%16)+48);lcd_data(':');
	   lcd_data((sec/16)+48);
	   lcd_data((sec%16)+48);delay_ms(105);
	   lcd_cmd(0XC0);
	   d = device_read(0xD0,0x04);						 //reading and displaying the date month and year
	   lcd_data(d/16+48);lcd_data(d%16+48);lcd_data('/');
	   m = device_read(0xD0,0x05);
	   lcd_data(m/16+48);lcd_data(m%16+48);lcd_data('/');
	   y = device_read(0xD0,0x06);
	   lcd_data(y/16+48);lcd_data(y%16+48);lcd_data('-');
	   dow = device_read(0XD0,0X03);					  //reading and displaying the day of the week
	   switch(dow){
	   	case 1:mes_display("SUN");break;
		case 2:mes_display("MON");break;
	   case 3:mes_display("TUES");break;
	   case 4:mes_display("WED");break;
	   case 5:mes_display("THURS");break;
	   case 6:mes_display("FRI");break;
	   case 7:mes_display("SAT");break;
	   }
}
}