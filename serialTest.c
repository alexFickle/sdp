#include <stdlib.h>
#include "lpc111x.h"
#include "armUtil.h"



int main() {
	GPIO2DIR = BIT11; //LED to show if powered/reset/in ISP mode
	GPIO2DATA = BIT11; // LED on
	uartInit();
	char rxc;
	// while(1){
		// while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
		// char rxc = U0RBR;
		// if(rxc == 'A') {
			// GPIO2DATA = 0; //off
			// for(volatile int i=0; i<1000000; i++){}; //delay
			// GPIO2DATA = BIT11; //on
		// } else {
			// U0THR = rxc;
		// }
	// }
	while(1){
		while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
		rxc = U0RBR;
		GPIO2DATA ^= BIT11;
		U0THR = rxc;	
	}
		
	return 0;
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