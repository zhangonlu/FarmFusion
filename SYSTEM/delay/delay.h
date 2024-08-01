#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f4xx.h"

//void delay_init(u8 SYSCLK);	//滴答定时器初始化
void delay_us(u32 us);		//微秒级延时
void delay_ms(u32 ms);		//毫秒级延时
void TIM7_Init(void);
#endif
