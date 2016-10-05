#include <stdlib.h>
#include "lpc111x.h"

void uartInit();


int main() {
	GPIO0DIR = BIT2; //LED to show if powered/reset/in ISP mode
	GPIO0DATA = BIT2; // LED on
	uartInit();
	while(1){
		while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
		char rxc = U0RBR;
		if(rxc == 'A') {
			GPIO0DATA = 0; //off
			for(volatile int i=0; i<1000000; i++){}; //delay
			GPIO0DATA = BIT2; //on
		} else {
			U0THR = rxc;
		}
	}
	return 0;
}

void uartInit() {
	
	SYSAHBCLKCTRL |= BIT16 | BIT6; //clock sent to IO
	
	IOCON_PIO1_6 &= ~0x07;
	IOCON_PIO1_6 |= 0x01; //Rx connected to pin
	IOCON_PIO1_7 &= ~0x07;
	IOCON_PIO1_7 |= 0x01; //Tx connected to pin
	
	SYSAHBCLKCTRL |= BIT12; //clock sent to UART
	
	UARTCLKDIV = 4; //clock has no prescaler
	
	U0LCR = 0x83; //8-bit, no parity, DLAB = 1
	U0DLL = 4; //baudrate
	U0FDR = 0x85; //115200 baud rate
	U0DLM = 0; //baudrate
	U0LCR = 0x03; //set DLAB = 0
	
	U0FCR = 0x07; //UART enabled
	return;
}


/* //known good: spams B
while(1){
		volatile int i;
		for(i=0; i<100000; i++){}; //delay
		U0THR = 'B';
	
	}
	*/
	
/* //known good: repeats char+1	
while(1){
		while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
		U0THR = U0RBR + 1;
		for(volatile int i = 0 ; i < 100000; i++){}; //delay
	}
	*/