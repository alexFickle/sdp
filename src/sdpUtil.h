#include <stdlib.h>

#ifndef SDPUTIL_H
#define SDPUTIL_H

void uint2hex(unsigned int number, char *destination);
//converts number to its 2 character representation and places it in destination
//destination[0] has the most significant nibble's hexadecimal  representation written to it
//destination[1] has the least significant nibble's hexadecimal  representation written to it
//if number > 255 it returns the hex representation of number % 256
//this first overflows binSet[0] for a binSet of size 73
//if additional cards are needed, use 3+ hex digits

char *buffer2hex(const unsigned int* buffer, unsigned int bufferSize);
//performs uint2hex on an entire buffer of unsigned integers
//returns a null terminated string of size 1+2*bufferSize

unsigned int hex2uint(const char *hex);
//hex must points to a characater array of at least length two
//hex[0] is the most significant nibble
//hex[1] is the least significant
//allowed values for the characters are 0123456789ABCDEF
//if hex[0] or hex[1] is not an allowed value a junk number between 0 and 255 is returned

void buffer2uint(const char *buffer, unsigned int bufferSize, unsigned int *destination);
//uses hex2uint to convert the hex character array buffer to the a uint buffer
//bufferSize is the number of hex pairs in buffer


#endif
