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
	struct IndexedBinSet *ibs = createIndexedBinSet(52);
	printBinSet(ibs->binSet);
	struct SortSpace *ss = newSortSpace();
	//TODO: send order to lift origin bin
	placeCards(ibs,ss);
	//TODO: handle errors in a loop (expose error bin to users, display issues, then placeCards on the error bin, repeat if needed)
	
	for(int i = 0; i < ibs->binSet[BS_NUM_INTER_BINS]; i++) {
		printf("\n%u :\n",i);
		liftBin(i,ss);
		printSortSpace(ss);
		emptyBin(i,ibs,ss);
		printf("\n\n");
		printSortSpace(ss);
		printf("\n---------------------\n");
	}
	deleteSortSpace(ss);
	deleteIndexedBinSet(ibs);
	free(listOrder);
	
	printf("\ndone!");
	return 0;
}




