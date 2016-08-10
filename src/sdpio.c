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
		printf("%2u ",list[i]);
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

char *loadListOrder(const char *listOrderFile) {
	char *delim = ",\0";
	unsigned int numCards;
	char *listOrderNoFormat = loadBuffer(listOrderFile);
	if(listOrderNoFormat == NULL) {
		return (char *)NULL; //list name does not have file of file failed to open
	}
	char *listOrder = (char *)malloc( sizeof(char) *((BS_MAX_NUM_CARDS * 2) + 1));
	char *token = strtok(listOrderNoFormat,delim);
	for(numCards = 0;token != NULL && numCards < BS_MAX_NUM_CARDS;numCards++) {
		listOrder[(numCards*2)] = *token;
		if(token[1] != '\0') {
			listOrder[(numCards*2) + 1] = token[1];
		} else {
			free(listOrderNoFormat);
			return (char *)NULL; //each element must have 2 characters
		}
		token = strtok(NULL,delim);
	}
	listOrder[(numCards*2)] = '\0';
	free(listOrderNoFormat);
	return listOrder;
}

void printListOrder(const char *listOrder) {
	for(unsigned int i = 0; listOrder[i] != '\0'; i+=2) {
		printf("%c%c\n",listOrder[i],listOrder[i+1]);
	}
	return;
}

void printSortSpace(const struct SortSpace *sortSpace) {
	for(int i = 0; i<4; i++) {
		printf("BIN  POS %2u:\n",i);
		printList(sortSpace->bin[i], sortSpace->cardCount[i]);
		printf("\n");
	}
	printf("LIFTED BIN:\n");
	printList(sortSpace->bin[4], sortSpace->cardCount[4]);
	return;
}

