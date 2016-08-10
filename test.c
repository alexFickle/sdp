#include "binSet.h"
#include "sdpUtil.h"
#include "sdpDefs.h"
#include "sdpio.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	unsigned int *binSet = constructBinSet(70);
	unsigned int binSetIndex[BS_MAX_INTER_BINS];
	struct IndexedBinSet *ibs = createIndexedBinSet(binSet,binSetIndex);
	printList(ibs->binSetIndex,ibs->binSet[BS_NUM_INTER_BINS]+1);
	printBinSet(binSet);
	for(int j=0; j<=5;j++) {
		for(int i=0;i<70;i++) {
			printf("%u,%2u: %2u\n",j,i,isCardAllowed(i,j,ibs));
		}
	}
	
	free(binSet);
	return 0;
}