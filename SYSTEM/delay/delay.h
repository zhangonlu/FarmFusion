#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f4xx.h"

//void delay_init(u8 SYSCLK);	//�δ�ʱ����ʼ��
void delay_us(u32 us);		//΢�뼶��ʱ
void delay_ms(u32 ms);		//���뼶��ʱ
void TIM7_Init(void);
#endif
