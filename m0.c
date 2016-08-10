#include "lpc1114.h"
#include <stdint.h>

int main(void) {
	GPIO0_MASK = BIT1;
	GPIO0_DIR  = BIT1;
	GPIO0_DATA = BIT1;
	while(1) {
		uint8_t i = 1;
	}
	return 0;
}