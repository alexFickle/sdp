#include "sdpio.h"

unsigned int *createRandList(unsigned int listSize) {
	unsigned int temp;       //stores value for swap opperations
	unsigned int randNum;    //random number
	unsigned int *randList = (unsigned int *)malloc(listSize * sizeof(unsigned int));
	srand(time(NULL));
	for(unsigned int i=0; i<listSize; i++) { //generates sequential list from 0 to listSize - 1
		randList[i] = i;
	}
	for(unsigned int i=0; i<listSize; i++) {
		randNum = rand() % ((listSize) -i);  //generate random index
		
		temp = randList[(listSize -1) -i];       //swaps value of random index with value of last index
		randList[(listSize -1) -i] = randList[randNum];
		randList[randNum] = temp;
	}
	return randList;
}

void printList(const unsigned int *list, unsigned int listSize) {
	for(unsigned int i=0; i<listSize; i++) {
		printf("%u \n",list[i]);
	}
	return;
}

void printHexList(const char *hexBuffer, unsigned int hexBufferSize) {
	for(unsigned int i=0; i<hexBufferSize; i+=2) {
		printf("%.2s\n",&hexBuffer[i]);
	}
	return;
}

void saveBuffer(const char *buffer,const char *fileName) {
	FILE *file = fopen(fileName,"w");
	if(file!=NULL)
	{
		fputs(buffer,file);
		fclose(file);
	}
	return;
}

char *loadBuffer(const char *fileName) {
	char *buffer = NULL;
	FILE *file = fopen(fileName,"r");
	if(file != NULL) {
		fseek(file,0L,SEEK_END); //move to EOF
		unsigned int fileSize = ftell(file) + 1; //get file size
		buffer = (char *)malloc(sizeof(char)*ftell(file));
		rewind(file); //move back to start of file
		fgets(buffer,fileSize,file); //copy file contents to string
		fclose(file);
	}
	return buffer;
}

