#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#ifndef SDPIO_H
#define SDPIO_H

void printList(const unsigned int *list, unsigned int listSize);
//prints a list of unsigned integers of size listSize using printf().  Prints 1 uint per line.

unsigned int *createRandList(unsigned int listSize);
//creates a list of uints containing the numbers 0 through (listSize-1) once in a random order.

void printHexList(const char *hexBuffer, unsigned int hexBufferSize);
//prints the hexBuffer in one pair per line
//hexBuffer does not need to be null terminated unless strlen(hexBuffer) is used to calc hexBufferSize

void saveBuffer(const char *buffer,const char *fileName);
//saves a char buffer as a file in the same dirrectory as the .exe with the given name
//the name should include an extension if it is desired
//will overwrite a previous file if it shares the same name
//buffer must be null terminated

char *loadBuffer(const char *fileName);
//returns the char buffer located in the given file
//returns NULL if the file does not exsist
//the returned buffer is null terminated


#endif