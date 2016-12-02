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
		if((movingMotor->position == 0) && (motorDirection < 0)) {
			movingMotor->position = movingMotor->num_steps - 1;
		} else if((movingMotor->position == movingMotor->num_steps - 1) && (motorDirection > 0)) {
			movingMotor->position = 0;
		} else {
			movingMotor->position += motorDirection;
		}
		
	}
	TMR16B0IR = BIT0; //rst the interrupt flag
    return;
}



void hardDelay(unsigned int centisec) {
	for(volatile int i = 0; i < centisec; i++) {
		for(volatile int j = 0; j < 10; j++) {
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

char readCommand() {
	LED2on();//status LED
	while((U0LSR & BIT0) == 0){}; //wait for new char to be recieved
	char cmd = U0RBR;
	hardDelay(4);
	U0THR = cmd;
	//GPIO0DATA ^= BIT11;
	LED2off();
	return cmd;
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

void moveWheel(struct MOTOR *motor, unsigned int cur, unsigned int desiredPOS) {
	unsigned int steps;
	int direction;
	//U0THR = '0'+ (motor->position / 100); //debug
	if(cur == desiredPOS) return;
	if(cur < desiredPOS)  {
		steps = desiredPOS - cur;
		direction = 1;
	} else {
		steps = cur - desiredPOS;
		direction = -1;
	}
	moveMotor(motor,steps,direction);
	waitForMotor();
	motor->position = desiredPOS;
	return;
}

//wheelMotor,liftMotor,dealerMotor
//cardDetector,liftDetector;
int main() {
	init();
	LED2off();//this LED indicates if the arm is waiting for a cmd from the pi
	moveMotor(&wheelMotor,2,1); //home to red tape bin
	waitForMotor();
	wheelMotor.position = 300;
	unsigned int curPos = 300;
	unsigned int firstGoFlag = 1;
	while(1) {
		char cmd = readCommand();
		switch(cmd) {
			case ('0'):
				moveWheel(&wheelMotor,curPos, 200);
				waitForMotor();
				curPos = 200;
				break;
			case ('1'):
				moveWheel(&wheelMotor,curPos, 100);
				waitForMotor();
				curPos = 100;
				break;
			case ('2'):
				moveWheel(&wheelMotor,curPos, 0);
				waitForMotor();
				curPos = 0;
				break;
			case ('3'):
				moveWheel(&wheelMotor,curPos, 300);
				waitForMotor();
				curPos = 300;
				break;
			/*case '1':
				moveWheel(&wheelMotor,100);
				waitForMotor();
				break;
			case '2':
				moveWheel(&wheelMotor,200);
				waitForMotor();
				break;
			case '3':
				moveWheel(&wheelMotor,300);
				waitForMotor();
				break;*/
			case '4':
				moveMotor(&wheelMotor,50,1);
				waitForMotor();
				curPos = (curPos+50)%400;
				break;
			case 'U':
				moveMotor(&liftMotor,10000,-1); //lift until any button press
				while(!(pollButton(&cardDetector) || pollButton(&liftDetector)));
				haltMotor(); //contact!!!
				unsigned int firstCardInStackFlag = 1;
				while(1) {//loop until break, until we are out of cards in the lifted bin
					moveMotor(&liftMotor,10000,-1); //lift until any button press
					while(!(pollButton(&cardDetector) || pollButton(&liftDetector)));
					haltMotor(); //contact!!!
					if(!pollButton(&liftDetector)) { //we have a cards, move it
						unsigned int cheatFactor = 0;
						if(firstCardInStackFlag == 1) {
							cheatFactor = 90;
							firstCardInStackFlag = 0;
						} else {
							cheatFactor = 0;
						}
						if(firstGoFlag == 0) {
							unsigned int c = (unsigned int) (readCommand()-'0');
							if(c == 4) break; //lift order overrules 
							unsigned int place = ((c%2 == 1) ? c*100 : ((c+2)%4)*100);
							
							moveWheel(&wheelMotor, curPos, place); //move so dest bin is selected
							waitForMotor();
							curPos = place;
						}
						moveMotor(&dealerMotor,245+cheatFactor,-1);
						waitForMotor();
						if(firstGoFlag == 1) {
							hardDelay(3);
							char readCmd = readCommand();
							//if(readCmd != 'P') break;
						}
						moveMotor(&liftMotor,20,1);
						waitForMotor();
					} else {
						moveMotor(&dealerMotor,500,-1);
						firstGoFlag = 0;
						waitForMotor();
						break;
					}
				}
				break;
			case 'D':
				moveMotor(&liftMotor,liftMotor.num_steps - liftMotor.position, 1); //drop lift
				waitForMotor();
				break;
			}
		}
	return 0;
}

/* //test super loop
while(1){
		while(1) {//loop until break, until we are out of cards in the lifted bin
			moveMotor(&liftMotor,10000,-1); //lift until any button press
			while(!(pollButton(&cardDetector) || pollButton(&liftDetector)));
			haltMotor(); //contact!!!
			if(!pollButton(&liftDetector)) { //we have a cards, move it
				moveMotor(&dealerMotor,220,-1);
				waitForMotor();
				moveMotor(&liftMotor,20,1);
				waitForMotor();
			} else { //no card, drop lift in a safe space
				moveMotor(&dealerMotor,500,-1);
				waitForMotor();
				hardDelay(1);
				break;
			}
		}
		moveMotor(&wheelMotor,50,1); //move to a drop zone
		waitForMotor();
		moveMotor(&liftMotor,liftMotor.num_steps - liftMotor.position, 1); //drop lift
		waitForMotor();
		moveMotor(&wheelMotor,50,1); //move to next bin
		waitForMotor();
	}


*/