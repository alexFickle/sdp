#include "sdpUtil.h"

//The following functions are used in this file and should not be needed elsewhere:
void createBinSetIndex(const unsigned int *binSet, unsigned int *binSetIndex);
//creates an index of max size BS_MAX_INTER_BINS + 1(see sdpDefs.h)
//constructs the binSetIndex of the IndexedBinSet structure

void uint2hex(unsigned int number, char *destination) {
	const char *hexDigits = "0123456789ABCDEF";
	destination[0] = hexDigits[(number >> 4) & 0x0F]; //MS nibble
	destination[1] = hexDigits[number & 0x0F]; //LS nibble
	return;
}

char *buffer2hex(const unsigned int *buffer, unsigned int bufferSize) {
	char * hexBuffer = (char *)malloc(1+2*bufferSize);
	for(unsigned int i=0; i<bufferSize; i++) {
		uint2hex(buffer[i],&hexBuffer[2*i]);
	}
	hexBuffer[2*bufferSize] = '\0';
	return hexBuffer;
}

unsigned int hex2uint(const char *hex) {
	const char *hexDigits = "0123456789ABCDEF";
	unsigned int number, counter;
	for( counter = 0 ;counter < 15 && hex[0] != hexDigits[counter]; counter++ );
	number = counter * 16;
	for( counter = 0 ;counter < 15 && hex[1] != hexDigits[counter]; counter++ );
	number += counter;
	return number;
}

void buffer2uint(const char *buffer, unsigned int bufferSize, unsigned int *destination) {
	for(int i=0;i<bufferSize;i++) {
		destination[i] = hex2uint(&buffer[2*i]);
	}
	return;
}

void createBinSetIndex(const unsigned int *binSet, unsigned int *binSetIndex) {
	unsigned int offset = 3;
	for(int i=0; i<binSet[BS_NUM_INTER_BINS]; i++) {
		binSetIndex[i] = offset;
		offset += (binSet[offset] + 1);
	}
	binSetIndex[binSet[BS_NUM_INTER_BINS]] = binSet[BS_SIZE];
	return;
}

struct IndexedBinSet createIndexedBinSet(unsigned int *binSet, unsigned int *binSetIndex) {
	struct IndexedBinSet ibs;
	if(binSet == NULL || binSetIndex == NULL || binSet[BS_NUM_INTER_BINS] > BS_MAX_INTER_BINS) {
		ibs.binSet = (unsigned int *)NULL;
		ibs.binSetIndex = (unsigned int *)NULL;
	} else {
		ibs.binSet = binSet;
		ibs.binSetIndex = binSetIndex;
		createBinSetIndex(binSet,binSetIndex);
	}
	return ibs;
}

unsigned int isCardAllowed(unsigned int card, unsigned int bin, struct IndexedBinSet ibs) {
	unsigned int lowerBound = ibs.binSetIndex[bin];
	unsigned int upperBound = ibs.binSetIndex[bin + 1];
	while((lowerBound+1) < upperBound) {
		unsigned int index = ((upperBound - lowerBound)/2) + lowerBound;
		if(ibs.binSet[index] > card) {
			upperBound = index;
		} else if(ibs.binSet[index] < card) {
			lowerBound = index;
		} else {
			return (unsigned int)1; //card found
		}
	}
	return (unsigned int)0; //card not found
}
