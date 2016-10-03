
#include <stdint.h>
#include "lpc111x.h"

void UARTInit(void);

void UARTInit(void){
	uint32_t regval;
	SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO      */
                      (1UL << 16) );  /* enable clock for IOCON     */
	IOCON_PIO1_6 &= ~0x07;
	IOCON_PIO1_6 = 0x01;	//configure UART RXD pin
	IOCON_PIO1_7 &= ~0x07;
	IOCON_PIO1_7 = 0x01;	//configure UART TXD pin
	SYSAHBCLKCTRL |= (1<<12); //enable clock to UART
	UARTCLKDIV |= (4<<0); //set clk divider to 68 (baud rate of 115200)
	U0FCR = BIT1;	//enable FIFO
	U0LCR = 0x03;	//set for 8 bit data width, 1 stop bit, no parity
	U0FDR = 0x85;
	U0DLM = 0; 
	U0TER = 0x80;	//enable transmission
	U0DLL = 4;
	
	U0FCR = 0x07; //enable and reset the Rx and Tx FIFO
	
	regval = U0LSR;
	regval = regval;
	return;
}
int main()
{
    //Turn on clock for GPIO, IOCON and ADC
    //SYSAHBCLKCTRL |= BIT6 + BIT13 + BIT16;
    GPIO0DIR = BIT8; // Make PIO0_8 an output
    //GPIO0DATA = 0;   // Turn off PIO0 outputs
	
	UARTInit();
	GPIO0DATA = BIT8;
	
	
	
	uint32_t input;
	int j,i;
    int n;
    while(1){

		while(1){
			if(U0LSR&0x01){;//wait for input
				break;
			}
		}
		
		GPIO0DATA = BIT8; // Turn on PIO0_8
		n=100000; while(--n);
		GPIO0DATA = 0;    // Turn on PIO0_8
		n=100000; while(--n); 
		input = input&U0RBR;
		if(U0LSR == (U0LSR&0xFE)){
			for(i=0; i<input; i++){
				GPIO0DATA = BIT8; // Turn on PIO0_8
				n=1000000; while(--n);
				GPIO0DATA = 0;    // Turn on PIO0_8
				n=1000000; while(--n); 
			}
		}
		// U0THR = 0xFF;
		// GPIO0DATA = 0;
		// n=10000; while(--n);		// delay
		// U0THR = 0x00;
		// GPIO0DATA = BIT8;
		// n=10000; while(--n);		// delay
    }    
}


