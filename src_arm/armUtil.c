#include "lpc111x.h"
#include "armUtil.h"

void setCoil(struct COIL coil) {
	*coil.port |= coil.pin;
	return;
}

void unsetCoil(struct COIL coil) {
	*coil.port &= ~coil.pin;
	return;
}

void stepMotor(struct MOTOR *motor, int direction) {
	if(direction > 0) {
		switch(motor->state) {
			case 0:
				setCoil(motor->B);
				break;
			case 1:
				unsetCoil(motor->A);
				break;
			case 2:
				setCoil(motor->C);
				break;
			case 3:
				unsetCoil(motor->B);
				break;
			case 4:
				setCoil(motor->D);
				break;
			case 5:
				unsetCoil(motor->C);
				break;
			case 6:
				setCoil(motor->A);
				break;
			case 7:
				unsetCoil(motor->D);
				break;
		}
		motor->state = (motor->state + 1) % 8;
	} else {
		switch(motor->state) {
			case 0:
				setCoil(motor->D);
				break;
			case 1:
				unsetCoil(motor->B);
				break;
			case 2:
				setCoil(motor->A);
				break;
			case 3:
				unsetCoil(motor->C);
				break;
			case 4:
				setCoil(motor->B);
				break;
			case 5:
				unsetCoil(motor->D);
				break;
			case 6:
				setCoil(motor->C);
				break;
			case 7:
				unsetCoil(motor->A);
				break;
		}
		motor->state = (motor->state + 7) % 8;
	}
	
	return;
}

void motorsInit(struct MOTOR *motor1) {
	timerInit();
	motor1Init(motor1); //init wheel motor
	//TODO: init motor 2 and 3
	return;
}

void motor1Init(struct MOTOR *motor) {
	IOCON_PIO3_5 = 0; //A
	IOCON_PIO0_6 = 0; //B
	IOCON_PIO0_7 = 0; //C
	IOCON_PIO2_9 = 0; //D
	GPIO0DIR |= (BIT6 | BIT7);
	GPIO2DIR |= BIT9;
	GPIO3DIR |= BIT5;
	motor->A.pin = 5;
	motor->A.port = &GPIO3DATA;
	motor->B.pin = 6;
	motor->B.port = &GPIO0DATA;
	motor->C.pin = 7;
	motor->C.port = &GPIO0DATA;
	motor->D.pin = 9;
	motor->D.port = &GPIO2DATA;
	motor->clock_top = 0;//TODO
	motor->num_steps = 0;//TODO
	motor->state = 0;//DONE
	setCoil(motor->A);
	return;
}

void timerInit() {
	SYSAHBCLKCTRL |= BIT7; //send main clk to CT16B0
	TMR16B0PR = 2200; //# of clock edges per TC count
	TMR16B0MCR = BIT0 | BIT1; //both reseting and interrupting on match 0
	TMR16B0MR0 = 2182; //# that the tc counts up to.
	TMR16B0CCR = 0; //no tc captures
	TMR16B0CTCR = 0; //timer mode
	TMR16B0TCR = BIT1; //at the end, rst counter
	ISER = BIT16;
	return;
}

void timerStart() {
	TMR16B0TCR = BIT0; //start the timer
	return;
}

void timerStop() {
	TMR16B0TCR = BIT1; //stop and rst the timer
	return;
}