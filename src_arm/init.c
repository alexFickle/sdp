#include "lpc111x.h"
void __init__(void);
void start(void)
{
	__init__();
}
void Default_Handler(void);

//if CT16B0_INTERRUPT is defined by passing interrupt=CT16B0_INTERRUPT to the makefile(can be set by default, see the makefile)
#ifdef CT16B0_INTERRUPT
extern void ISR_CT16B0(void);
#undef CT16B0_INTERRUPT //this is to surpress a warning for redefining 
#define CT16B0_INTERRUPT ISR_CT16B0
#endif
#ifndef CT16B0_INTERRUPT
#define CT16B0_INTERRUPT Default_Handler
#endif
extern int main();

// The following are 'declared' in the linker script
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;
extern void isr_spi1(void);
// the section "vectors" is placed at the beginning of flash 
// by the linker script
const void * Vectors[] __attribute__((section(".vectors"))) ={
	(void *)0x10002000, 	/* Top of stack */ 
	__init__,   		/* Reset Handler */
	Default_Handler,	/* NMI */
	Default_Handler,	/* Hard Fault */
	0,	                /* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	Default_Handler,	/* SVC */
	0,                   	/* Reserved */
	0,                   	/* Reserved */
	Default_Handler,     	/* PendSV */
	Default_Handler,     	/* SysTick */		
/* External interrupt handlers follow */
	Default_Handler, 	/* PIO0_0 */
	Default_Handler, 	/* PIO0_1 */
	Default_Handler, 	/* PIO0_2 */
	Default_Handler, 	/* PIO0_3 */
	Default_Handler, 	/* PIO0_4 */
	Default_Handler, 	/* PIO0_5 */
	Default_Handler, 	/* PIO0_6 */
	Default_Handler, 	/* PIO0_7 */
	Default_Handler, 	/* PIO0_8 */
	Default_Handler, 	/* PIO0_9 */
	Default_Handler, 	/* PIO0_10 */
	Default_Handler, 	/* PIO0_11 */
	Default_Handler,	/* PIO1_0 */
	Default_Handler ,  	/* C_CAN */
	Default_Handler, 	/* SSP1 */
	Default_Handler, 	/* I2C */
	//Default_Handler, 	/* CT16B0 */  
	CT16B0_INTERRUPT,	/* CT16B0 */
	Default_Handler, 	/* CT16B1 */
	Default_Handler, 	/* CT32B0 */
	Default_Handler, 	/* CT32B1 */
	Default_Handler, 	/* SSP0 */
	Default_Handler,	/* UART */
	Default_Handler, 	/* RESERVED */
	Default_Handler, 	/* RESERVED */
	Default_Handler, 	/* ADC */
	Default_Handler, 	/* WDT */
	Default_Handler, 	/* BOD */
	Default_Handler, 	/* RESERVED */
	Default_Handler, 	/* PIO3 */
	Default_Handler, 	/* PIO2 */
	Default_Handler, 	/* PIO1 */
	Default_Handler 	/* PIO0 */
};
void __clock_init__()
{
	// This function sets the main clock to the PLL
	// The PLL input is the built in 12MHz RC oscillator
	// This is multiplied up to 48MHz for the main clock
	// MSEL = 3, PSEL = 1 see page 51 of UM10398 user manual
	SYSPLLCLKSEL = 0; // select internal RC oscillator
	SYSPLLCTRL = (3 << 0) | (1 << 5); // set divisors/multipliers
	PDRUNCFG &= ~BIT7; // Power up the PLL.
	SYSPLLCLKUEN = 1; // inform PLL of update
	
	MAINCLKSEL = 3; // Use PLL as main clock
	MAINCLKUEN = 1; // Inform core of clock update
}
void __init__()
{
// do global/static data initialization
	unsigned char *src;
	unsigned char *dest;
	unsigned len;
	src= &INIT_DATA_VALUES;
	dest= &INIT_DATA_START;
	len= &INIT_DATA_END-&INIT_DATA_START;
	while (len--)
		*dest++ = *src++;
// zero out the uninitialized global/static variables
	dest = &BSS_START;
	len = &BSS_END - &BSS_START;
	while (len--)
		*dest++=0;
	__clock_init__();
	main();
}

void Default_Handler()
{
	while(1);
}