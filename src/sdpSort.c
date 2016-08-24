#include "sdpSort.h"


void createBinSetIndex(const unsigned int *binSet, unsigned int *index) {
	unsigned int offset = 3;
	for(unsigned int i=0; i<binSet[BS_NUM_INTER_BINS]; i++) {
		index[i] = offset;
		offset += (binSet[offset] + 1);
	}
	index[binSet[BS_NUM_INTER_BINS]] = binSet[BS_SIZE];
	return;
}

struct IndexedBinSet *createIndexedBinSet(unsigned int numCards) {
	struct IndexedBinSet *ibs = (struct IndexedBinSet *)malloc(sizeof(struct IndexedBinSet));
	ibs->index = (unsigned int *)malloc(sizeof(unsigned int) * BS_MAX_INTER_BINS);
	ibs->binSet = constructBinSet(numCards);
	if(ibs->binSet == NULL || ibs->index == NULL || ibs->binSet[BS_NUM_INTER_BINS] > BS_MAX_INTER_BINS) {
		ibs->binSet = (unsigned int *)NULL;
		ibs->index = (unsigned int *)NULL;
	} else {
		createBinSetIndex(ibs->binSet,ibs->index);
	}
	return ibs;
}

void deleteIndexedBinSet(struct IndexedBinSet *ibs) {
	if(ibs->index != NULL) {
		free(ibs->index);
	}
	if(ibs->binSet != NULL) {
		free(ibs->binSet);
	}
	return;
}

unsigned int isCardAllowed(unsigned int card, unsigned int bin, const struct IndexedBinSet *ibs) {
	unsigned int lowerBound = ibs->index[bin];
	unsigned int upperBound = ibs->index[bin + 1];
	while((lowerBound+1) < upperBound) {
		unsigned int index = ((upperBound - lowerBound)/2) + lowerBound;
		if(ibs->binSet[index] > card) {
			upperBound = index;
		} else if(ibs->binSet[index] < card) {
			lowerBound = index;
		} else {
			return (unsigned int)1; //card found
		}
	}
	return (unsigned int)0; //card not found
}

struct SortSpace *newSortSpace() {
	struct SortSpace *sortSpace = (struct SortSpace*)malloc(sizeof(struct SortSpace));
	sortSpace->bulk = (unsigned int *)malloc(5 * BS_MAX_NUM_CARDS * sizeof(unsigned int));
	resetSortSpace(sortSpace);
	return sortSpace;
}

void resetSortSpace(struct SortSpace *sortSpace) {
	for(unsigned int i = 0; i<5; i++) {
		sortSpace->bin[i] = &sortSpace->bulk[i * BS_MAX_NUM_CARDS];
		sortSpace->cardCount[i] = 0;
	}
	return;
}

void deleteSortSpace(struct SortSpace* sortSpace) {
	if(sortSpace != NULL) {
		if(sortSpace->bulk != NULL) {
			free(sortSpace->bulk);
		}
		free(sortSpace);
	}
	return;
}

unsigned int cardToPosition(const char card[2], const char *listOrder) {
	unsigned int position;
	for(position = 0; position < (sizeof(listOrder)/2);position++) {
		if(card[0] == listOrder[2*position] && card[1] == listOrder[(2*position) + 1]) {
			return position;
		}
	}
	return BS_MAX_NUM_CARDS;
}

unsigned int destinationBin(unsigned int cardID, const struct IndexedBinSet *ibs, unsigned int liftedBinNumber) {
	for(unsigned int i = 0; i <= 4 && (liftedBinNumber + i) < ibs->binSet[BS_NUM_INTER_BINS]; i++) {
		if(isCardAllowed(cardID,(liftedBinNumber + i),ibs)) {
			return (liftedBinNumber + i) % 4;
		}
	}
	return (ibs->binSet[BS_NUM_INTER_BINS]) % 4; //place in final bin
}

void placeCard(unsigned int cardID, const struct IndexedBinSet *ibs, struct SortSpace* sortSpace) {
	unsigned int destBin = destinationBin(cardID, ibs, 0);
	sortSpace->bin[destBin][sortSpace->cardCount[destBin]] = cardID;
	sortSpace->cardCount[destBin] ++;
	return;
}

void moveCard(unsigned int cardID, unsigned int liftedBinNumber, const struct IndexedBinSet *ibs, struct SortSpace* sortSpace) {
	unsigned int destBin = destinationBin(cardID, ibs, liftedBinNumber);
	//TODO: send cmd to get 1 card(w/out reading it)
	//TODO: send cmd to move wheel so destBin will recieve the card
	//TODO: send cmd to deposit the card
	sortSpace->bin[destBin][sortSpace->cardCount[destBin]] = cardID;
	sortSpace->cardCount[destBin] ++;
	sortSpace->cardCount[4] --;
	return;
}

void liftBin(unsigned int bin, struct SortSpace *sortSpace) {
	bin = bin % 4;
	unsigned int *temp = sortSpace->bin[4];
	//TODO: send cmd to move wheel so spatula can lowerBound, wait until done
	//TODO: send cmd to lower spatula, wait until done
	//TODO: send cmd to move spatula under new sorce bin, wait until done
	//TODO: send cmd to raise spatula, wait until done
	sortSpace->bin[4] = sortSpace->bin[bin];
	sortSpace->bin[bin] = temp;
	sortSpace->cardCount[4] = sortSpace->cardCount[bin];
	sortSpace->cardCount[bin] = 0;
	return;
}

void emptyBin(unsigned int liftedBinNumber, const struct IndexedBinSet *ibs, struct SortSpace* sortSpace) {
	liftedBinNumber = liftedBinNumber+1;
	while(sortSpace->cardCount[4] > 0) {
		unsigned int cardID = sortSpace->bin[4][sortSpace->cardCount[4] - 1];
		moveCard(cardID, liftedBinNumber, ibs, sortSpace);
	}
	return;
}

#define ERROR_VALUE 255 //temp, ignoring errors
unsigned int dummyReadCard(const struct IndexedBinSet *ibs, struct SortSpace *ss) { //temp, for debug, will be replaced with call to image analysis program
	static unsigned int numCardsPlaced = 0;
	static unsigned int *list = NULL;
	if(numCardsPlaced == 0) {
		list = createRandList(ibs->binSet[BS_NUM_CARDS]);
	} else if(numCardsPlaced >= ibs->binSet[BS_NUM_CARDS]) {
		free(list);
		list = NULL;
		numCardsPlaced = 0;
		return ibs->binSet[BS_NUM_CARDS];
	}
	//printf("%2u: %u\n",numCardsPlaced,list[numCardsPlaced]); //debug print
	numCardsPlaced +=1;
	return list[numCardsPlaced];
}

void placeCards(const struct IndexedBinSet *ibs, struct SortSpace *ss) {
	//TODO: send cmd to move origin bin to lift position
	//TODO: send cmd to lift the origin bin
	while(1) {
		//TODO: move 1 card over the camera
		//TODO: read the card
		unsigned int card = dummyReadCard(ibs,ss); //temp for debug, replace with call to image analysis program
		if(card == ERROR_VALUE) {
		//TODO: catch error from card read
		}
		else if(card < ibs->binSet[BS_NUM_CARDS]) {
			placeCard(card,ibs,ss);
		}
		else {
			break; //end of list
		}
	}
	return;
}

