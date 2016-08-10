#include "sdpSort.h"

void createBinSetIndex(const unsigned int *binSet, unsigned int *binSetIndex) {
	unsigned int offset = 3;
	for(int i=0; i<binSet[BS_NUM_INTER_BINS]; i++) {
		binSetIndex[i] = offset;
		offset += (binSet[offset] + 1);
	}
	binSetIndex[binSet[BS_NUM_INTER_BINS]] = binSet[BS_SIZE];
	return;
}

struct IndexedBinSet *createIndexedBinSet(unsigned int *binSet, unsigned int *binSetIndex) {
	struct IndexedBinSet *ibs = (struct IndexedBinSet *)malloc(sizeof(struct IndexedBinSet));
	if(binSet == NULL || binSetIndex == NULL || binSet[BS_NUM_INTER_BINS] > BS_MAX_INTER_BINS) {
		ibs->binSet = (unsigned int *)NULL;
		ibs->binSetIndex = (unsigned int *)NULL;
	} else {
		ibs->binSet = binSet;
		ibs->binSetIndex = binSetIndex;
		createBinSetIndex(binSet,binSetIndex);
	}
	return ibs;
}

unsigned int isCardAllowed(unsigned int card, unsigned int bin, const struct IndexedBinSet *ibs) {
	unsigned int lowerBound = ibs->binSetIndex[bin];
	unsigned int upperBound = ibs->binSetIndex[bin + 1];
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
	free(sortSpace->bulk);
	free(sortSpace);
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
	sortSpace->bin[destBin][sortSpace->cardCount[destBin]] = cardID;
	sortSpace->cardCount[destBin] ++;
	sortSpace->cardCount[4] --;
	return;
}

void liftBin(unsigned int bin, struct SortSpace *sortSpace) {
	bin = bin % 4;
	unsigned int *temp = sortSpace->bin[4];
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