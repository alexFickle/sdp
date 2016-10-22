See headerfiles for function declarations and full documentaion

TODO: rewrite this; a lot has changed, but the header file docs are up to date.

binSet:
A binSet is a unsigned integer array that contains all of the information needed to sort a stack of cards.
binSet.c/.h contains functions used to create (but not use) this binSet
This code is inteneded to be ran only on the pi.
The only function that will be needed on the final project is constructBinSet( unsiged integer numCards)
	which returns an unsigned integer pointer to a binSet for numCards cards
Also contains printBinSet(unsigned int *binSet) which is used as a debug print

sdpUtil:
Contains functions used on the pi and/or the arm.
These functions are used to convert between hex and unsigned integer.
Also inculdes functions used in the sorting process

sdpio:
Contians print functions used mainly in the debugging of the program.
Also contains file functions that can be used to save and load binSets or other data

sdpDefs.h:
Contains definitions used with binSets.
Will be expanded to inculde definitions useful to other parts of the program
	i.e.: #define BIT1 0x01

sdpPinout.h:
Contains the pin information of the arm processor.  Not used by any code currently, just used as a reference.

lpc111x.h:
Contains information about the lpc1114 register locations.

compilation:
make: currently compiles test.c (along with any dependencies that need updating)
make test: compiles test.c along with its dependecies
make clean: deletes all object files, executables and test text files,
	on Windows make clean is tested and deletes files.
	make clean is untested with Linux and currently only displays all files instead of deleting;
	see Makefile on directions on how to change it once it is tested.
"target=arm" can be appended to any command to compile the target into a hex file that can be uploaded
	to the cortex m0 using arm-none-eabi-gcc from https://launchpad.net/gcc-arm-embedded/
	arm-none-eabi-gcc must be in your computer's path.  You must also set LINKER_PATH in the Makefile
	the location of lgcc.a; will be something like: ...\GNU Tools ARM Embedded\5.3 2016q1\lib\gcc\arm-none-eabi\5.3.1\armv6-m.

FOR ARM ONLY:
interrupt="0_INTERRUPT 1_INTERRUPT ..." can be appended to enable any desired interrupts.  This overwrites any default interrupts.
"interrupt=" can be appended to turn off all interrupts.

sample compilations:
make 
make simSort target=pi
make BlinkArm target=arm interrupt=
make motorTest target=arm interrupt=CT16B0_INTERRUPT