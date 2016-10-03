#include <stdlib.h>
#include "lpc111x.h"


int main() {
	int i;
	GPIO0DIR = BIT2;
	while(1){
		GPIO0DATA = 0;
		for(i=0; i<=1000; i++){}; //delay
		GPIO0DATA = BIT2;
		for(i=0; i<=1000; i++){}; //delay
	}
	return 0;
}