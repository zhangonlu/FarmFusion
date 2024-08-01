#include "beep.h"

// BEEP -----> PB10 ------>AHB1
//高电平驱动
void BEEP_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}


