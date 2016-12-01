#include "lpc111x.h"
#include "armUtil.h"
#include "math.h"

struct MOTOR wheelMotor,liftMotor,dealerMotor;

struct BUTTON{
	struct COIL power;
	struct COIL sense;
};

struct BUTTON cardDetector,liftDetector;

//cardDetector
void button1init(struct BUTTON *but) {
	//PIO0_8 : power
	IOCON_PIO0_8 = 0; //check this
	GPIO0DIR |= BIT8;
	but->power.pin = BIT8;
	but->power.port = &GPIO0DATA;
	but->power.reversedPolarity = 0;
	setCoil(but->power);
	//PIO0_9 : sense
	IOCON_PIO0_9 = BIT3; //check this, want pull down resistor
	GPIO0DIR &= ~BIT9;
	but->sense.pin = BIT9;
	but->sense.port = &GPIO0DATA;
	but->sense.reversedPolarity = 0;
	return;
}

//liftDetector
void button2init(struct BUTTON *but) {
	//PIO0_10 : power
	IOCON_SWCLK_PIO0_10 = BIT0; //check this
	GPIO0DIR |= BIT10;
	but->power.pin = BIT10;
	but->power.port = &GPIO0DATA;
	but->power.reversedPolarity = 0;
	setCoil(but->power);
	//PIO1_10 : sense
	IOCON_PIO1_10 = BIT3|BIT7; //check this, want pull down resistor
	GPIO1DIR &= ~BIT10;
	but->sense.pin = BIT10;
	but->sense.port = &GPIO1DATA;
	but->sense.reversedPolarity = 0;
	return;
}

unsigned int pollButton(struct BUTTON *but) {
	return ((*but->sense.port & but->sense.pin) == 0) ? 0:1;
}

void ISR_CT16B0(void) {
	
	if(stepsToCount == 0) { //we have stepped the required amount
		timerStop();
		movingMotor = NULL;
	} else { //step the motor once
		stepMotor(movingMotor,motorDirection);
		stepsToCount--;
		movingMotor->position += motorDirection;
	}
	TMR16B0IR = BIT0; //rst the interrupt flag
    return;
}

void init() {
	motorsInit(&wheelMotor,&liftMotor,&dealerMotor);
	movingMotor = NULL; //from armUtil.h
	stepsToCount = 0;
	LED2init();
	LED3init();
	uartInit();
	button1init(&cardDetector);
	button2init(&liftDetector);
}

void hardDelay(unsigned int sec) {
	for(volatile int i = 0; i < sec; i++) {
		for(volatile int j = 0; j < 500; j++) {
			for(volatile int k = 0; k < 20000; k++) {
			}
		}
	}
}

void LED2off() {
	GPIO2DATA &= ~BIT11; // LED off
	return;
}

void LED2on() {
	GPIO2DATA |= BIT11; // LED on
	return;
}
/*
unsigned int readCommand() {
	
	while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
}
*/
//wheelMotor,liftMotor,dealerMotor
//cardDetector,liftDetector;
int main() {
	init();
	moveMotor(&wheelMotor,2,1);
	waitForMotor();
	while(1){
		while(1) {//loop until break
			moveMotor(&liftMotor,10000,-1); //lift until button press
			while(!(pollButton(&cardDetector) || pollButton(&liftDetector)));
			haltMotor(); //contact!!!
			if(!pollButton(&liftDetector)) { //we have a cards, move it
				moveMotor(&dealerMotor,203,-1);
				waitForMotor();
				//moveMotor(&liftMotor,10,1);
				waitForMotor();
			} else { //no card, drop lift in a safe space
				moveMotor(&dealerMotor,203,-1);
				waitForMotor();
				break;
			}
		}
		moveMotor(&wheelMotor,50,1); //move to a drop zone
		waitForMotor();
		moveMotor(&liftMotor,fabs(liftMotor.position), (liftMotor.position > 0) ? 1:-1); //drop lift
		waitForMotor();
		moveMotor(&wheelMotor,50,1); //move to next bin
		waitForMotor();
	}
	return 0;
}