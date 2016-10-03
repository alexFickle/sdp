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

//GPIO1 definitions:
#define GPIO1_offset *(uint16_t *)0x50010000 //offset for GPIO1 registers
#define GPIO1_MASK   *(uint16_t *)0x50010000 //mask R/W
#define GPIO1_DATA   *(uint16_t *)0x50013FFC //data R/W
#define GPIO1_DIR    *(uint16_t *)0x50018000 //dirrection(I/O) R/W
#define GPIO1_IS     *(uint16_t *)0x50018004 //interupt sense R/W
#define GPIO1_IBE    *(uint16_t *)0x50018008 //interrupt both edge R/W
#define GPIO1_IEV    *(uint16_t *)0x5001800C //inteerupt event R/W
#define GPIO1_IE     *(uint16_t *)0x50018010 //interrupt mask R/W
#define GPIO1_RIS    *(uint16_t *)0x50018014 //raw interrupt status R
#define GPIO1_MIS    *(uint16_t *)0x50018018 //masked interrupt status R
#define GPIO1_IC     *(uint16_t *)0x5001801C //interrupt clear W

//GPIO2 definitions:
#define GPIO2_offset *(uint16_t *)0x50020000 //offset for GPIO2 registers
#define GPIO2_MASK   *(uint16_t *)0x50020000 //mask R/W
#define GPIO2_DATA   *(uint16_t *)0x50023FFC //data R/W
#define GPIO2_DIR    *(uint16_t *)0x50028000 //dirrection(I/O) R/W
#define GPIO2_IS     *(uint16_t *)0x50028004 //interupt sense R/W
#define GPIO2_IBE    *(uint16_t *)0x50028008 //interrupt both edge R/W
#define GPIO2_IEV    *(uint16_t *)0x5002800C //inteerupt event R/W
#define GPIO2_IE     *(uint16_t *)0x50028010 //interrupt mask R/W
#define GPIO2_RIS    *(uint16_t *)0x50028014 //raw interrupt status R
#define GPIO2_MIS    *(uint16_t *)0x50028018 //masked interrupt status R
#define GPIO2_IC     *(uint16_t *)0x5002801C //interrupt clear W

//GPIO3 definitions:
#define GPIO3_offset *(uint16_t *)0x50030000 //offset for GPIO3 registers
#define GPIO3_MASK   *(uint16_t *)0x50030000 //mask R/W
#define GPIO3_DATA   *(uint16_t *)0x50033FFC //data R/W
#define GPIO3_DIR    *(uint16_t *)0x50038000 //dirrection(I/O) R/W
#define GPIO3_IS     *(uint16_t *)0x50038004 //interupt sense R/W
#define GPIO3_IBE    *(uint16_t *)0x50038008 //interrupt both edge R/W
#define GPIO3_IEV    *(uint16_t *)0x5003800C //inteerupt event R/W
#define GPIO3_IE     *(uint16_t *)0x50038010 //interrupt mask R/W
#define GPIO3_RIS    *(uint16_t *)0x50038014 //raw interrupt status R
#define GPIO3_MIS    *(uint16_t *)0x50038018 //masked interrupt status R
#define GPIO3_IC     *(uint16_t *)0x5003801C //interrupt clear W


#endif
