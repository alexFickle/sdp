#include "sdpUtil.h"


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


