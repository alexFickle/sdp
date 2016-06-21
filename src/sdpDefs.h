#ifndef SDPDEFS_H
#define SDPDEFS_H

#ifndef NULL
#define NULL (void *)0
#endif

//binSet definitions
#define BS_SIZE 0
#define BS_MAX_SIZE 256
#define BS_NUM_CARDS 1
#define BS_MAX_NUM_CARDS 70
#define BS_NUM_INTER_BINS 2
#define BS_MAX_INTER_BINS 10
#define BS_SORT_BUFFER_SIZE (BS_MAX_INTER_BINS*BS_MAX_NUM_CARDS)
#define BS_UART_BUFFER_SIZE (2*BS_MAX_MEM)

#endif