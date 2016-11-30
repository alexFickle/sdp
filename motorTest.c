#include "lpc111x.h"
#include "armUtil.h"

struct MOTOR wheelMotor,liftMotor,dealerMotor;
struct MOTOR *movingMotor; //the motor that is currently being moved
unsigned int flag = 0;

void ISR_CT16B0(void) {
    static unsigned int i = 0;
    if(i<200) {
        stepMotor(movingMotor, -1);
        i++;
    } else if(i < 300){
        i++;
    }
    else if(i < 500) {
        stepMotor(movingMotor, 1);
        i++;
    }
    else if(i < 900){
        i++;
    }
	else{
		i = 0;
		flag = (flag+1) % 3;
	}

    TMR16B0IR = BIT0; //rst the interrupt flag
    return;

}

void init() {
	motorsInit(&wheelMotor,&liftMotor,&dealerMotor);
	LED2init();
	LED3init();
	//uartInit();
}

int main() {
	init();
	movingMotor = &wheelMotor;
	//timerStart();
	while(1){
		while(flag ==0);
		movingMotor = &dealerMotor;
		while(flag ==1);
		movingMotor = &liftMotor;
		while(flag ==2);
		movingMotor = &wheelMotor;
	} //loop forever
	return 0;
}