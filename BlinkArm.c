
#include"lpc111x.h"


int main() {
	volatile int i;
	GPIO0DIR = BIT2;
	IOCON_PIO0_2 = 0;
	
	while(1){
		GPIO0DATA = 0;
		for(i=0; i<1000000; i++){}; //delay
		GPIO0DATA = BIT2;
		for(i=0; i<1000000; i++){}; //delay
	}
	return 0;
}