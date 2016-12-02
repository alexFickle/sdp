#include "lpc111x.h"
#include "armUtil.h"

void setCoil(struct COIL coil) {
	if(coil.reversedPolarity == 0) {
		*coil.port |= coil.pin;
	} else {
		*coil.port &= ~coil.pin;
	}
	return;
}

void unsetCoil(struct COIL coil) {
	if(coil.reversedPolarity == 0) {
		*coil.port &= ~coil.pin;
	} else {
		*coil.port |= coil.pin;
	}
	return;
}

void stepMotor(volatile struct MOTOR *motor, int direction) {
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

void motorsInit(struct MOTOR *motor1, struct MOTOR *motor2, struct MOTOR *motor3) {
	timerInit();
	motor1Init(motor1);
	motor2Init(motor2);
	motor3Init(motor3);
	return;
}

//wheel
void motor1Init(struct MOTOR *motor) {
	IOCON_PIO3_5 = 0; //A
	IOCON_PIO0_6 = 0; //B
	IOCON_PIO0_7 = 0; //C
	IOCON_PIO2_9 = 0; //D
	GPIO0DIR |= (BIT6 | BIT7);
	GPIO2DIR |= BIT9;
	GPIO3DIR |= BIT5;
	motor->A.pin = BIT5;
	motor->A.port = &GPIO3DATA;
	motor->B.pin = BIT6;
	motor->B.port = &GPIO0DATA;
	motor->C.pin = BIT7;
	motor->C.port = &GPIO0DATA;
	motor->D.pin = BIT9;
	motor->D.port = &GPIO2DATA;
	motor->clock_top = 21820;
	motor->num_steps = 400;//test this
	motor->state = 0;//start with just coil A conducting.
	motor->A.reversedPolarity = 0;
	motor->B.reversedPolarity = 0;
	motor->C.reversedPolarity = 0;
	motor->D.reversedPolarity = 0;
	setCoil(motor->A);
	unsetCoil(motor->B);
	unsetCoil(motor->C);
	unsetCoil(motor->D);
	motor->position = 0;
	return;
}

//lift
void motor2Init(struct MOTOR *motor) {
    IOCON_PIO2_1 = 0; //A
    IOCON_PIO0_3 = 0; //B
    IOCON_PIO0_4 = 0; //C
    IOCON_PIO0_5 = 0; //D
    GPIO0DIR |= (BIT3 | BIT4 | BIT5);
    GPIO2DIR |= BIT1;
    motor->A.pin = BIT1;
    motor->A.port = &GPIO2DATA;
    motor->B.pin = BIT3;
    motor->B.port = &GPIO0DATA;
    motor->C.pin = BIT4;
    motor->C.port = &GPIO0DATA;
    motor->D.pin = BIT5;
    motor->D.port = &GPIO0DATA;
    motor->clock_top = 21000;
    motor->num_steps = 800;//test this
    motor->state = 0;//start with just coil A conducting.
    motor->A.reversedPolarity = 0;
	motor->B.reversedPolarity = 0;
	motor->C.reversedPolarity = 0;
	motor->D.reversedPolarity = 0;
	setCoil(motor->A);
	unsetCoil(motor->B);
	unsetCoil(motor->C);
	unsetCoil(motor->D);
	motor->position = 0;
    return;
}

//dealer
void motor3Init(struct MOTOR *motor) {
    IOCON_PIO1_8 = 0; //A
    IOCON_PIO0_2 = 0; //B
    IOCON_PIO2_7 = 0; //C
    IOCON_PIO2_8 = 0; //D
    GPIO0DIR |= BIT2;
    GPIO1DIR |= BIT8;
    GPIO2DIR |= (BIT7 | BIT8);
    motor->A.pin = BIT8;
    motor->A.port = &GPIO1DATA;
    motor->B.pin = BIT2;
    motor->B.port = &GPIO0DATA;
    motor->C.pin = BIT7;
    motor->C.port = &GPIO2DATA;
    motor->D.pin = BIT8;
    motor->D.port = &GPIO2DATA;
    motor->clock_top = 5000;
    motor->num_steps = 800;//test this
    motor->state = 0;//start with just coil A conducting.
    motor->A.reversedPolarity = 0;
	motor->B.reversedPolarity = 0;
	motor->C.reversedPolarity = 0;
	motor->D.reversedPolarity = 0;
	setCoil(motor->A);
	unsetCoil(motor->B);
	unsetCoil(motor->C);
	unsetCoil(motor->D);
	motor->position = 0;
    return;
}


void timerInit() {
    SYSAHBCLKCTRL |= BIT7; //send main clk to CT16B0
    TMR16B0PR = 20; //# of clock edges per TC count
    TMR16B0MCR = BIT0 | BIT1; //both reseting and interrupting on match 0
    TMR16B0MR0 = 21820; //# that the tc counts up to.
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

//PIO2_11
void LED2init(){
	GPIO2DIR |= BIT11; //set output
	IOCON_PIO2_11 = 0; //set function to GPIO, no pull up/down resistor
	GPIO2DATA |= BIT11; // LED on
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

void uartInit() {
	
	SYSAHBCLKCTRL |= BIT16 | BIT6; //clock sent to IO
	
	IOCON_PIO1_6 &= ~0x07;
	IOCON_PIO1_6 |= 0x01; //Rx connected to pin
	IOCON_PIO1_7 &= ~0x07;
	IOCON_PIO1_7 |= 0x01; //Tx connected to pin
	
	SYSAHBCLKCTRL |= BIT12; //clock sent to UART
	
	UARTCLKDIV = 4; //clock has no prescaler
	
	U0LCR = 0x83; //8-bit, no parity, DLAB = 1
	U0DLL = 4; //baudrate
	U0FDR = 0x85; //115200 baud rate
	U0DLM = 0; //baudrate
	U0LCR = 0x03; //set DLAB = 0
	
	
	U0FCR = 0x07; //UART enabled
	return;
}

void moveMotor(struct MOTOR *motor,unsigned int steps, int direction) {
	TMR16B0MR0 = motor->clock_top;
	stepsToCount = steps;
	motorDirection = direction;
	movingMotor = motor;
	timerStart();
	return;
}

void waitForMotor() {
	while(movingMotor != NULL);
	return;
}

void haltMotor() {
	stepsToCount = 0;
	movingMotor = NULL;
	return;
}
