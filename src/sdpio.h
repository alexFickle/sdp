#ifndef SDPIO_H
#define SDPIO_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "sdpDefs.h"
#include "sdpUtil.h"
#include "sdpSort.h"




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

char *loadListOrder(const char *listOrderFile);
//Loads the desired order of the cards from a file passed by listOrderFile
//Each element in the list is two characters deliminated by a comma
//The first characteer is a number 2,...,9 or X(10), J,Q,K,A 
//The second character is either the letter H,S,C,D which represents the suit
//returns a nul terminated string

void printListOrder(const char *listOrder);
//prints the list order, one card(two characters) per line
//requires listOrder to be nul terminated

void printSortSpace(const struct SortSpace *sortSpace);
//prints a sort space


#endif