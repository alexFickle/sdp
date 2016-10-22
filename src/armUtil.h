#include "lpc111x.h"

#ifndef ARM_UTIL_H
#define ARM_UTIL_H

struct COIL {
	unsigned int pin;
	volatile unsigned int *port;
};
//used in the motor structure, represents a pin GPIO pin on the arm that gets amplified with the ULN2003a

void setCoil(struct COIL coil);
//sets the GPIO line high, causes the corresponding darlington pair in the ULN2003a to conduct, causing the motor coil to conduct
void unsetCoil(struct COIL coil);
//sets the GPIO line low, causes the corresponding darlington pair in the ULN2003a to not conduct, causing the motor coil to not conduct


struct MOTOR {
	struct COIL A;
	struct COIL B;
	struct COIL C;
	struct COIL D;
	unsigned int clock_top;
	unsigned int num_steps;
	unsigned int state;
};
//stucture that represents a 4 phase stepper motor, the coils are driven high in the following pattern:
/* MOTOR STATES:
0: A
1: AB
2: B
3: BC
4: C
5: CD
6: D
7: DA
*/

void stepMotor(struct MOTOR *motor, int direction);
//steps the given motor in the given dirrection by one step

void motorsInit(struct MOTOR *motor1);
//inits motors 1-3 and the timer used to control the motor step frequency

void motor1Init(struct MOTOR *motor);
//inits motor1

void timerInit() ;
//inits the timer so it can be used to control the motor step frequency
//the timer used in named CT16B0 (Counter Timer 16 bit 0)

void timerStart();
//starts the timer so that the periodic interrupt fires

void timerStop();
//stops and zeros the timer to pause the periodic interrupt





#endif