#include "lpc111x.h"
#include "armUtil.h"

void ISR_CT16B0(void) {
	GPIO2DATA = GPIO2DATA ^ BIT11;
	TMR16B0IR = BIT0;
	return;
	
}


int main() {
	//struct MOTOR motor1; //wheel motor
	//motorsInit(&motor1);
	GPIO2DIR = BIT11; //LED init
	IOCON_PIO2_11 = 0;
	GPIO2DATA = 0; // LED off
	timerInit();
	timerStart();
	while(1){} //loop forever
	return 0;
}