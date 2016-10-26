#include "lpc111x.h"
#include "armUtil.h"

struct MOTOR *movingMotor; //the motor that is currently being moved
struct COIL LED2_coil; //test

void ISR_CT16B0(void) {
	static unsigned int i = 0;
	if(i==0) {
		setCoil(LED2_coil);
		i = 1;
	} else {
		unsetCoil(LED2_coil);
		i = 0;
	}
	//GPIO0DATA = GPIO0DATA ^ BIT11;
	
	TMR16B0IR = BIT0; //rst the interrupt flag
	return;
	
}

//PIO2_11
void LED2init(){
	GPIO2DIR |= BIT11; //set output
	IOCON_PIO2_11 = 0; //set function to GPIO, no pull up/down resistor
	GPIO2DATA &= ~BIT11; // LED off
	return;
}

//PIO0_11
void LED3init(){
	SYSAHBCLKCTRL |= (BIT16);
	GPIO0DIR |= BIT11; //set output
	IOCON_R_PIO0_11 &= ~0x07;
	IOCON_R_PIO0_11 |= 0x01;
	GPIO0DATA |= BIT11; // LED on
	return;
}




int main() {
	struct MOTOR motor1; //wheel motor
	LED2_coil.pin = BIT11;
	LED2_coil.port = &GPIO2DATA;
	motorsInit(&motor1);
	LED2init();
	LED3init();
	timerStart();
	while(1){} //loop forever
	return 0;
}