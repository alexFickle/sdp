#include "lpc111x.h"
#include "armUtil.h"

struct MOTOR *wheelMotor = NULL;
struct MOTOR *liftMotor = NULL;
struct MOTOR *dealerMotor = NULL;
struct MOTOR *movingMotor; //the motor that is currently being moved

void ISR_CT16B0(void) {
    static unsigned int i = 0;
    if(i<400) {
        stepMotor(movingMotor, -1);
        i++;
    } else if(i < 500){
        i++;
    }
    else if(i < 900) {
        stepMotor(movingMotor, 1);
        i++;
    }
    else {
        i = (i+1) % 1200;
    }

    TMR16B0IR = BIT0; //rst the interrupt flag
    return;

}

void init() {
	motorsInit(wheelMotor,liftMotor,dealerMotor);
	LED2init();
	LED3init();
	uartInit();
}

int main() {
	init();
	timerStart();
	while(1){} //loop forever
	return 0;
}