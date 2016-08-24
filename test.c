#include "binSet.h"
#include "sdpUtil.h"
#include "sdpDefs.h"
#include "sdpio.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	struct IndexedBinSet *ibs = createIndexedBinSet(70);
	printList(ibs->index,ibs->binSet[BS_NUM_INTER_BINS]+1);
	printBinSet(ibs->binSet);
	for(int j=0; j<=5;j++) {
		for(int i=0;i<70;i++) {
			printf("%u,%2u: %2u\n",j,i,isCardAllowed(i,j,ibs));
		}
	}
	
	deleteIndexedBinSet(ibs);
	return 0;
}