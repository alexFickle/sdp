#include "lpc111x.h"

#ifndef NULL
#define NULL (void *)0;
#endif

#ifndef ARM_UTIL_H
#define ARM_UTIL_H

struct COIL {
	unsigned int pin;
	volatile unsigned int *port;
	unsigned int reversedPolarity;
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
	unsigned int num_steps; // # of steps in a full rotation
	unsigned int state;
	unsigned int position; //[0 ... num_steps)
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
/* Alternate coil names:
A: 1+, Black
B: 2+, Red
C: 1-, Green
D: 2-, Blue
*/

void stepMotor(struct MOTOR *motor, int direction);
//steps the given motor in the given dirrection by one step

void motorsInit(struct MOTOR *motor1, struct MOTOR *motor2, struct MOTOR *motor3);
//inits motors 1-3 and the timer used to control the motor step frequency

void motor1Init(struct MOTOR *motor);
//inits motor1, currently wheel motor

void motor2Init(struct MOTOR *motor);
//inits motor2, currently lift motor

void motor3Init(struct MOTOR *motor);
//inits motor3, currently dealer motor

void timerInit() ;
//inits the timer so it can be used to control the motor step frequency
//the timer used in named CT16B0 (Counter Timer 16 bit 0)

void timerStart();
//starts the timer so that the periodic interrupt fires
//interrupt frequncy = main_clk / (TMR16B0PR * TMR16B0MR0)
//the main_clk frequency is currently 48MHz.

void timerStop();
//stops and zeros the timer to pause the periodic interrupt

void LED2init();
//inits LED2 on the board

void LED3init();
//inits LED3 on the board

void uartInit();
//inits the UART
//curently interrupts are not enabled for this
//TODO




#endif