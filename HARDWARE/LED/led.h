#ifndef __LED_H_
#define __LED_H_


#include "sys.h"

typedef struct {
	BitAction Led1;
	BitAction Led2;
	BitAction Led3;
}LED_State;


#define LED1  PEOut(8)
#define LED2  PEOut(9)
#define LED3  PEOut(10)

extern u8 led1_state;
extern u8 led3_state;
void LED_Init(void);



#endif
