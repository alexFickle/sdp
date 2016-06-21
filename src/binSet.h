#include <stdio.h>
#include <stdlib.h>
#include "sdpDefs.h"

#ifndef BINSET_H
#define BINSET_H

//A binSet contains all of the necessary information to sort a stack of cards.
//The sorting method used is to take all of the cards from bin 0 and putting each card
//starting from the top of the stack and putting them in the first bin that has the card
//in its allowed set.
//This process is repeated for all bins until all of the cards are in the final bin.

unsigned int *constructBinSet(unsigned int numCards);
//Constructs a binSet
//binSet[0] is the size of the binSet in sizeof(unsiged int)
//binSet[1] is numCards
//binSet[2] is the number of intermediate bins used to sort the stack
//binSet[3] is the size of the first bin's allowed set
//The next binSet[3] items (binSet[4], ... , binSet[3 + binSet[3]]) are the first bin's allowed set
//The next item is the size of the second bin's allowed set and is followed by the second bin's allowed set if applicable (binSet[1] > 1)
//This pattern continues for all of the intermediate bins.

void printBinSet(const unsigned int *binSet);
//Debug print for a binSet.  Uses printf().

#endif

/* example usage:

#include "binSet.h"
//#include "sdpio.h"

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printBinSet(constructBinSet(52));
		printBinSet(constructBinSet(13));
	} else {
		for(int i = 1;i<argc;i++) {
			unsigned int numCards = (unsigned int)atoi(argv[i]);
			unsigned int *binSet = constructBinSet(numCards);
			if(binSet != NULL) {			
				printBinSet(binSet);
				//printf("Unformated print for %u:\n",numCards);
				//printList(binSet,binSet[0]);
			}
			free(binSet);
		}
	}
	return 0;
}
*/