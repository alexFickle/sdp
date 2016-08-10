#include "binSet.h"
#include "sdpDefs.h"
#include "sdpio.h"
#include "sdpUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>



int main(int argc, char *argv[]) {
	char *listOrder = loadListOrder("hearts.csv");
	printListOrder(listOrder);
	unsigned int *binSet = constructBinSet(52);
	printBinSet(binSet);
	unsigned int binSetIndex[BS_MAX_INTER_BINS];
	struct IndexedBinSet *ibs = createIndexedBinSet(binSet,binSetIndex);
	struct SortSpace *ss = newSortSpace();
	for(int i = 0; i< 52; i++) {
		placeCard(i,ibs,ss);
	}
	
	for(int i = 0; i < binSet[BS_NUM_INTER_BINS]; i++) {
		printf("\n%u :\n",i);
		liftBin(i,ss);
		printSortSpace(ss);
		emptyBin(i,ibs,ss);
		printf("\n\n");
		printSortSpace(ss);
		printf("\n---------------------\n");
	}
	
	
	return 0;
}




