
#include"lpc111x.h"


int main() {
	volatile int i;
	GPIO2DIR = BIT11;
	IOCON_PIO2_11 = 0;
	
	while(1){
		GPIO2DATA = 0;
		for(i=0; i<1000000; i++){}; //delay
		GPIO2DATA = BIT11;
		for(i=0; i<1000000; i++){}; //delay
	}
	return 0;
}