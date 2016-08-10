#ifndef LPC1114_H
#define LPC1114_H

#include <stdint.h>

//bit mask definitions:
#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

//GPIO0 definitions:
#define GPIO0_offset *(uint16_t *)0x50000000 //offset for GPIO0 registers
#define GPIO0_MASK   *(uint16_t *)0x50000000 //mask R/W
#define GPIO0_DATA   *(uint16_t *)0x50003FFC //data R/W
#define GPIO0_DIR    *(uint16_t *)0x50008000 //dirrection(I/O) R/W
#define GPIO0_IS     *(uint16_t *)0x50008004 //interupt sense R/W
#define GPIO0_IBE    *(uint16_t *)0x50008008 //interrupt both edge R/W
#define GPIO0_IEV    *(uint16_t *)0x5000800C //inteerupt event R/W
#define GPIO0_IE     *(uint16_t *)0x50008010 //interrupt mask R/W
#define GPIO0_RIS    *(uint16_t *)0x50008014 //raw interrupt status R
#define GPIO0_MIS    *(uint16_t *)0x50008018 //masked interrupt status R
#define GPIO0_IC     *(uint16_t *)0x5000801C //interrupt clear W




#endif
