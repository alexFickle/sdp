#include <stdlib.h>
#include "lpc111x.h"


int main() {
	GPIO0DIR = BIT2;
	volatile char *letter = (volatile char *) U0RBR;
	U0FCR = BIT0; //UART enabled
	U0LCR = BIT0 | BIT1 | BIT7; //8-bit, no parity, letter is readable
	U0FDR = BIT0| BIT2 | BIT7; //115200 baud rate
	U0DLL = 9; //baudrate
	U0DLM = 0; //baudrate
	while(1){
		GPIO0DATA = 0;
		while(*letter != 'A'); //delay
		GPIO0DATA = BIT2;
		while(*letter == 'A'); //delay
	}
	return 0;
}