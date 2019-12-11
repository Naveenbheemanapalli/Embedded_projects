/********************************************************
*	UART BIT - BANGING CODE FOR 8051(AT89S52) micro- controller
*	
*	baud rate = 9600bps	@ 12MHZ clk frequency
*
*	if you want to use the different baud rate then change
*
*	the value of TH1 register of Timer1	of micro Controller
*
*	Thank Q
*********************************************************/
#include<reg51.h>
typedef unsigned char u8;
sbit tx = P3^6;					//using the port3 - 6th pin as the transmitter pin for uart communication
sbit rx = P3^7;					//using the port3 - 7th pin as the receiver pin for uart communication
void baud_delay(){				//delay between the bit and bit for the maintining baud rate between Transmitter and Reciever
	TR1=1;
	while(TF1==0);
	TR1=0;
	TF1=0;
}

void serial_transmit(u8 temp){
	signed char i=7;
	tx = 0;						  //start bit transmitting
	baud_delay();
	while(i>=0){				  //transmitting 8 bit data serially
	tx= (temp&(1<<i))? 1:0;		  
	baud_delay();
	i--;
	}
	baud_delay();
	tx = 1;			   			//stop bit transmitting
}

u8 serial_receive(){
	u8 dta=0x00;
	signed char i;
	start :
	if(rx == 0);				//checking for the start bit in the pin rx
	else
		goto start;
	baud_delay();
	for(i=7;i>=0;i--){			//getting the 8 bits form the reciever pin
		if(rx==1){
			dta=dta|(1<<i);
		}
	baud_delay();
	}
	baud_delay();			   //waiting for the stop bit delay
return dta;
}

void main(){
	u8 temp;TMOD = 0x20;TH1=151; 	//baud setting(9600) if U need different baud rate change value of TH1
	TR1=1;
	while(TF1==0);TR1=0;TF1=0; 		//overflowing the timer for first condition for accuracy
	P3 = 0xFF;
	temp = 'A';
	serial_transmit(temp);		   //transmitting data A serially
	temp = serial_receive();	   //recieving data serially and storing that data into temp
	P2 = temp;						//transferring temp to PORT2 of micro-controller
	while(1);
}
