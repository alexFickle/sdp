See headerfiles for function declarations and full documentaion

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



compilation:
make: currently compiles test.c (along with any dependencies that need updating)
make test: compiles test.c along with its dependecies
make clean: deletes all object files, executables and test text files,
	on Windows make clean is tested and deletes files.
	make clean is untested with Linux and currently only displays all files instead of deleting;
	see Makefile on directions on how to change it once it is tested.

make currently requires a folder named bin in the same directory. It will fail if it does not.
I will eventually fix this, but for now create this folder yourself.

