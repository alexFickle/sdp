See headerfiles for function declarations and full documentaion

-----------------------------------------------------------------------------------------------------------
PI code:

binSet:
A binSet is a unsigned integer array that contains all of the information needed to sort a stack of cards.
binSet.c/.h contains functions used to create (but not use) this binSet
This code is inteneded to be ran only on the pi.
The only function that will be needed on the final project is constructBinSet( unsiged integer numCards)
	which returns an unsigned integer pointer to a binSet for numCards cards
Also contains printBinSet(unsigned int *binSet) which is used as a debug print

sdpDefs.h:
Contains definitions used with binSets.

sdpUtil:
Contains utility functions used on the pi.
These functions are used to convert between hex and unsigned integer.

sdpSort:
Contains sort code that uses a binSet to sort the deck of cards.

sdpio:
Contians print functions used mainly in the debugging of the program.
Also contains file functions that can be used to save and load binSets or other data


-----------------------------------------------------------------------------------------------------------
ARM code:

armUtil:
Contains functions that initilize periphials and move motors.

init:
Contains the clock init function and the interrupt vector.
Interrupts must be declared as extern in this file if they are used
and their function pointer placed in the vector.
The extern declaration is handled by the makefile for each interrupt that we have used so far.
See the interrupt section in the Makefile docuentaion to select which interrrupts are enabled.

linker_script.ld:
A script used by the linker, do not modify.

lpc111x.h:
Contains information about the lpc1114 register locations.


-----------------------------------------------------------------------------------------------------------
Makefile:

You can compile a program that contains a main() for both the ARM and PI using this makefile.
The program being compiled must be in the same directory as the makefile.

Target selection:
"target=arm" can be appended to any command to compile the target into a hex file that can be uploaded
	to the cortex m0 using arm-none-eabi-gcc from https://launchpad.net/gcc-arm-embedded/
	arm-none-eabi-gcc must be in your computer's path.  You must also set LINKER_PATH in the Makefile
	the location of lgcc.a; will be something like: 
		...\GNU Tools ARM Embedded\5.3 2016q1\lib\gcc\arm-none-eabi\5.3.1\armv6-m.
"target=pi" can be used to compile the program for either a raspberry pi or any other computer using gcc.

Interrupt selection (FOR ARM USE ONLY):
interrupt="0_INTERRUPT 1_INTERRUPT ..." can be appended to enable any desired interrupts.  
	This overwrites any default interrupts.
"interrupt=" can be appended to turn off all interrupts.

The names of interrupts currenty availiable: CT16B0_INTERRUPT

For each " "_INTERRUPT that is eanabled you must write a void ISR_" "(void).
For example CT16B0_INTERRUPT has a corresponding function void ISR_CT16B0().


-----------------------------------------------------------------------------------------------------------
sample compilations:
make 
make simSort target=pi
make BlinkArm target=arm interrupt=
make motorTest target=arm interrupt=CT16B0_INTERRUPT

see the Makefile for which program compiles by default.