#ifndef SDP_PINOUT_H
#define SDP_PINOUT_H

#ifdef LQFP48
//LPC1114FBD481301

//power: 
//VDD: pin_8, pin_44
//VSS: pin_5, pin_41

//pi-arm gpio:
#define PI_GPIO_PORT 0 //PIN_4
#define PI_GPIO_PIN  1
//TXD: pin_47
//RXD: pin_46
//RST: pin_3


//sense:
#define SENSE_0_PORT 0  //PIN_32
#define SENSE_0_PIN  11
#define SENSE_0_ADC  0

#define SENSE_1_PORT 1  //PIN_33
#define SENSE_1_PIN  0
#define SENSE_1_ADC  1

#define SENSE_2_PORT 1  //PIN_34
#define SENSE_2_PIN  1
#define SENSE_2_ADC  2

#define SENSE_3_PORT 1  //PIN_35
#define SENSE_3_PIN  2
#define SENSE_3_ADC  3


//motors:
#define MOTOR_1A_PORT 1   //PIN_9
#define MOTOR_1A_PIN  8
#define MOTOR_1B_PORT 0   //PIN_10
#define MOTOR_1B_PIN  2
#define MOTOR_1C_PORT 2   //PIN_11
#define MOTOR_1C_PIN  7
#define MOTOR_1D_PORT 2   //PIN_12
#define MOTOR_1D_PIN  8

#define MOTOR_2A_PORT 2   //PIN_13
#define MOTOR_2A_PIN  1
#define MOTOR_2B_PORT 0   //PIN_14
#define MOTOR_2B_PIN  3
#define MOTOR_2C_PORT 0   //PIN_15
#define MOTOR_2C_PIN  4
#define MOTOR_2D_PORT 0   //PIN_16
#define MOTOR_2D_PIN  5

#define MOTOR_3A_PORT 3   //PIN_21
#define MOTOR_3A_PIN  5
#define MOTOR_3B_PORT 0   //PIN_22
#define MOTOR_3B_PIN  6
#define MOTOR_3C_PORT 0   //PIN_23
#define MOTOR_3C_PIN  7
#define MOTOR_3D_PORT 2   //PIN_23
#define MOTOR_3D_PIN  9


#endif

#ifdef DIP28
//LPC1114FN28/102
//TODO
#endif


#endif