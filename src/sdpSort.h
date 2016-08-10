#include <stdlib.h>
#include "sdpDefs.h"

#ifndef SDP_SORT_H
#define SDP_SORT_H

struct IndexedBinSet {
	unsigned int *binSet;
	unsigned int *binSetIndex;
};
//object given to isCardAllowed()

struct IndexedBinSet *createIndexedBinSet(unsigned int *binSet, unsigned int *binSetIndex);
//constructs the object given to isCardAllowed()

unsigned int isCardAllowed(unsigned int card, unsigned int bin, const struct IndexedBinSet *ibs);
//determines if the card with sort id card is in the allowed set of bin
//return of 0 indicates the card is not allowed, 1 indicates it is allowed

struct SortSpace {
	unsigned int *bin[5]; //bin[BIN_NUMBER][POS_IN_BIN], bin4 is always lifted
	unsigned int cardCount[5];
	unsigned int *bulk;
};
//keeps tract of the location of each card as it is being sorted

struct SortSpace *newSortSpace();
//creates a SortSpace and resets it so that it is usable

void resetSortSpace(struct SortSpace *sortSpace);
//call after an entire deck is sorted to be able to reuse the object in a future sort

void deleteSortSpace(struct SortSpace* sortSpace);
//frees a SortSpace

unsigned int cardToPosition(const char card[2], const char *listOrder);
//given the two character representation of a card and a valid list order,
//returns an unsigned integer representing the cards index in the list order, this is the cardID used in a binset
//returns BS_MAX_NUM_CARDS if the card is not in the list order

unsigned int destinationBin(unsigned int cardID, const struct IndexedBinSet *ibs, unsigned int liftedBinNumber);
//returns the next valid bin[0,4] that the card is allowed.

void placeCard(unsigned int cardID, const struct IndexedBinSet *ibs, struct SortSpace* sortSpace);
//used after a card is read using the 
void emptyBin(unsigned int liftedBinNumber, const struct IndexedBinSet *ibs, struct SortSpace* sortSpace);
void liftBin(unsigned int bin, struct SortSpace *sortSpace);




#endif