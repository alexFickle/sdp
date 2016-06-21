#include "binSet.h"

//The following functions are used in the construction of the binSet and should not be needed elsewhere
void printPaths(const unsigned int *paths,unsigned int numPaths, unsigned int pathSize);
//Debug print. Prints a list of paths using printf().  One path per line.

int isPathValid(unsigned int *path, unsigned int pathSize);
//Checks if a path is valid.  A path can not have either four consecuative zeros nor three leading zeros to be valid.

int pathCompare(const void *path1, const void *path2);
//Used with the qsort function to sort the paths with the fewest number of card opperations to the bottom of a path list.
//Must be prepared before the qsort() with a pathCompare(Null,(void *)&pathSize) with pathSize being an unsigned integer.

unsigned int *constructPaths(unsigned int numPaths, unsigned int *pathSize);
//Creates a list of paths with the minimum path size.
//Unsinged int paths[Numpaths][PathSize] <-- this is how my 2D array works

unsigned int *simSort(const unsigned int *paths, unsigned int numPaths, unsigned int pathSize);
//Sorts a list of paths so that the paths' indexes match the position that an object that is given that path would appear in the sorted list
//TODO: make a better sim sort

unsigned int pathCost(const unsigned  int *paths, unsigned int numPaths, unsigned int pathSize);
//Calculates the number of card transfer opperations involved with a path list.
//DO NOT touch this function, used to determine how much memory needs to be allocated for a binSet.
//Also will be used to determine if it is benificial to over bin.

unsigned int *pathToBinSet(const unsigned int *orderedPaths,unsigned int numPaths, unsigned int pathSize);
//Creates a binSet from the path list to be transmitted to the cortex m0, only input ordered path lists.

void printPaths(const unsigned int *paths, unsigned int numPaths, unsigned int pathSize) {
	for(unsigned int i = 0; i < numPaths; i++) {
		for(unsigned int j = 0; j < pathSize; j++) {
			if( paths[i*pathSize + j] == 1) {
				printf("1");
			} else {
				printf("0");
			}
		}
		printf("\n");
	}
	return;
}

int isPathValid(unsigned int *path, unsigned int pathSize) {
	unsigned int consecutiveStates[4] = {0,0,1,0};
	for(unsigned int i=0; i<pathSize;i++) {
		consecutiveStates[0] = consecutiveStates[1]; //downshift
		consecutiveStates[1] = consecutiveStates[2];
		consecutiveStates[2] = consecutiveStates[3];
		consecutiveStates[3] = path[i];  //load path from right
		if(consecutiveStates[0] == 0 && consecutiveStates[1] == 0 && consecutiveStates[2] == 0 && consecutiveStates[3] == 0) {
			return 0; //state invalid
		}
	}
	return 1; //state is valid
}

int pathCompare(const void *path1, const void *path2) {
	static unsigned int pathSize = 1;
	int dif = 0; //number of 1s in path1 - number of paths in path2
	if(path1 == NULL) { //set pathSize before using inside qsort
		pathSize = *(unsigned int *)path2;
	} else {
		for(unsigned int i=0; i<pathSize; i++) {
			dif += ((unsigned int *)path1)[i];
			dif -= ((unsigned int *)path2)[i];
		}
	}
	return dif;
}

unsigned int *constructPaths(unsigned int numPaths, unsigned int *pathSize) {
	unsigned int *newPaths = (unsigned int *)malloc(2*1*sizeof(unsigned int)); //inintially there is only 1 intermediate bin
	unsigned int *oldPaths;
	*pathSize = 1; // number of intermediate bins
	unsigned int newNumPathsFound = 2; //initialzing base case
	unsigned int oldNumPathsFound;
	newPaths[0] = 0; // two possible paths that comes from having one intermediate bin
	newPaths[1] = 1;
	while(newNumPathsFound < numPaths) {
		oldPaths = newPaths; // new case failed, make it old
		oldNumPathsFound = newNumPathsFound;
		newNumPathsFound = 0;
		*pathSize = *pathSize + 1;  //throw more bins at the problem
		newPaths = (unsigned int *)malloc(2*oldNumPathsFound*(*pathSize)*sizeof(unsigned int)); //and more RAM
		for(unsigned int i=0; i<oldNumPathsFound;i++) {
			for(unsigned int j=0; j<(*pathSize-1); j++) {
				newPaths[newNumPathsFound*(*pathSize) + j] = oldPaths[i*((*pathSize)-1) + j];
			}
			newPaths[(newNumPathsFound+1)*(*pathSize) - 1] = 0;
			newNumPathsFound += isPathValid(&newPaths[newNumPathsFound*(*pathSize)],(*pathSize)); //incriment path counter only if valid path
			for(unsigned int j=0; j<(*pathSize-1); j++) {
				newPaths[newNumPathsFound*(*pathSize) + j] = oldPaths[i*((*pathSize)-1) + j];
			}
			newPaths[(newNumPathsFound+1)*(*pathSize) - 1] = 1;
			newNumPathsFound++; //concatenating 1 always results in a valid path
		}
		free(oldPaths); //drop old path list
	} 
	//now we have enough unique valid paths
	//next I sort the paths so the ones with the highest number of ones are on bottom
	pathCompare((void *)NULL,pathSize); //prep cmp function for qsort
	qsort((void *)newPaths,newNumPathsFound,(*pathSize * sizeof(unsigned int)), pathCompare); //sort
	
	//now I create final path list that will be returned:
	unsigned int *paths = (unsigned int *)malloc(numPaths * *pathSize * sizeof(unsigned int));
	for(int i=0; i<(numPaths * *pathSize); i++) { //and copy the data over up to the number of paths asked for
		paths[i] = newPaths[i];
	}
	free(newPaths); //clean up!
	return paths;
}

unsigned int *simSort(const unsigned int *paths, unsigned int numPaths, unsigned int pathSize) {
	unsigned int *bins = (unsigned int *)malloc( sizeof(unsigned int) * (pathSize+2)*(numPaths * pathSize)); //bin list excluding error bin.
	for(unsigned int i=0; i<(numPaths * pathSize); i++) { //copy given path lists into bin0
		bins[i] = paths[i];
	}
	int numCards[numPaths + 2]; //keeps track of number of cards in each bin
	numCards[0] = numPaths; //bin0 has all cards
	for(unsigned int i=1; i<pathSize + 2;i++) { //all other bins have no cards
		numCards[i] = 0;
	}
	
	for(unsigned int sourceBinIndex = 0; sourceBinIndex <= pathSize; sourceBinIndex++) { //loop over all non-final bins as the source bin
		for(unsigned int cardIndex = numPaths - numCards[sourceBinIndex]; cardIndex < numPaths ; cardIndex ++) { //keep looping until the source bin is out of cards
			unsigned int sourceIndex = (numPaths * pathSize * sourceBinIndex) + (pathSize * cardIndex); //start of card to be sorted
			unsigned int destinationBinIndex; // index of bin that the card is being moved to, must be greater than sourceBinIndex
			//below for loop calculates destinationBinIndex, runs no code in loop besides check and incriment
			for(destinationBinIndex = sourceBinIndex + 1; bins[sourceIndex + destinationBinIndex - 1] == 0 && destinationBinIndex <= pathSize; destinationBinIndex++); //calcs first bin that source card can be sorted into
			unsigned int destinationIndex = (numPaths * pathSize * (destinationBinIndex + 1)) - ((numCards[destinationBinIndex] + 1) * pathSize); //start of destination card position
			for(unsigned int copyCounter = 0; copyCounter < pathSize; copyCounter ++) { //copy card into its new home
				bins[destinationIndex + copyCounter] = bins[sourceIndex + copyCounter];
			}
			numCards[destinationBinIndex] ++; //update card counter
		}
		
	}
	
	unsigned int *binf = (unsigned int *)malloc(numPaths * pathSize * sizeof(unsigned int)); //final bin to be returned
	for(unsigned int i=0;i<numPaths * pathSize; i++) {
		binf[i] = bins[i +((pathSize+1)*(numPaths * pathSize))];
	}
	free(bins);
	return binf;
	
}

unsigned int pathCost(const unsigned  int *paths, unsigned int numPaths, unsigned int pathSize) {
	unsigned int pathCost = numPaths;
	for(unsigned int pathIndex=0; pathIndex<numPaths; pathIndex++) {
		for(unsigned int positionInPath=0; positionInPath<pathSize; positionInPath++){
			pathCost += paths[(pathIndex * pathSize) + positionInPath];
		}
	}
	return pathCost;
	
}

unsigned int *pathToBinSet(const unsigned int *orderedPaths,unsigned int numPaths, unsigned int pathSize) {
	unsigned int binSize = pathCost(orderedPaths,numPaths,pathSize) - numPaths + pathSize + 3;
	unsigned int *binSet = (unsigned int *)malloc(binSize * sizeof(unsigned int));
	unsigned int allowedCount; //allowed number of paths in the current bin
	unsigned int startOfBinIndex = 3; //the start of the current bin
	binSet[0] = binSize; //Bin size
	binSet[1] = numPaths; //number of cards
	binSet[2] = pathSize; //number of bins
	for(unsigned int binIndex = 0;binIndex < pathSize; binIndex++){ //loop over every bin 
		allowedCount = 0;
		for(unsigned int pathIndex = 0; pathIndex < numPaths; pathIndex++) { //loop over every card
			if(orderedPaths[(binIndex + (pathIndex * pathSize))] == 1) {
				binSet[startOfBinIndex + allowedCount + 1] = pathIndex;
				allowedCount++;
			}
		}
		binSet[startOfBinIndex] = allowedCount;
		startOfBinIndex += allowedCount + 1;
	}
	return binSet;
}

unsigned int *constructBinSet(unsigned int numCards) {
	if(numCards == 0) return (unsigned int *)NULL;
	unsigned int numBins = 0; //modified by constructPaths, number of intermediate bins
	unsigned int *pathList = constructPaths(numCards,&numBins); //create paths
	unsigned int *orderedPathList = simSort(pathList,numCards,numBins);
	free(pathList);
	unsigned int *binSet = pathToBinSet(orderedPathList,numCards,numBins);
	free(orderedPathList);
	return binSet;
}

void printBinSet(const unsigned int *binSet) {
	if(binSet == (unsigned int *)NULL) return;
	printf("\n\nBIN SET PRINT:\n");
	printf("SET SIZE:  %3u\n",binSet[BS_SIZE]);
	printf("NUM CARDS: %3u\n",binSet[BS_NUM_CARDS]);
	printf("NUM BINS:  %3u",binSet[BS_NUM_INTER_BINS]+3);
	printf("\nBIN0: %2u allowed values: ALL VALUES",binSet[2]);
	printf("\nBIN1:  1 allowed values: ERROR VALUE");
	unsigned int printIndex = 3;
	unsigned int startOfBinIndex;
	for(unsigned int binIndex = 0; binIndex < binSet[BS_NUM_INTER_BINS]; binIndex++){
		printf("\nBIN%u: %2u allowed values:",binIndex+2,binSet[printIndex]);
		startOfBinIndex = printIndex;
		printIndex++;
		while(printIndex <= (startOfBinIndex + binSet[startOfBinIndex])) {
			printf(" %2u",binSet[printIndex]);
			printIndex ++;
		}
	}
	printf("\nBIN%u: %2u allowed values: ALL VALUES",binSet[BS_NUM_INTER_BINS]+2,binSet[2]);
	printf("\nEND OF BIN SET PRINT \n\n");
	return;
}