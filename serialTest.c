#include <stdlib.h>
#include "lpc111x.h"

void uartInit();
void echo();


int main() {
	GPIO0DIR = BIT2; //LED to show if powered/reset/in ISP mode
	GPIO0DATA = BIT2; // LED on
	uartInit();
	while(1){
		echo();
	}
	return 0;
}

void uartInit() {
	SYSAHBCLKCTRL = BIT12; //clock sent to uart
	U0FCR = BIT0; //UART enabled
	U0LCR = BIT0 | BIT1; //8-bit, no parity, letter is readable
	//U0FDR = BIT0| BIT2 | BIT7; //115200 baud rate
	U0TER |= 0x80;
	//U0DLL = 4; //baudrate
	//U0DLM = 0; //baudrate
	IOCON_PIO1_6 = 0x1; //Rx connected to pin
	IOCON_PIO1_7 = 0x1; //Tx connected to pin
	GPIO1DIR = BIT7; //PORT 1 PIN 7 (TXD) is an output
	UARTCLKDIV = 104; //system clock sent to UART.
	return;
}

void echo() {
    //while (!(U0LSR & 0x01));    //waiting for new char
	//U0THR = U0RBR; 			//read and send char
	volatile int i;
	for(i=0; i<100000; i++){}; //delay
	U0THR = 0x41;
	return;
}
