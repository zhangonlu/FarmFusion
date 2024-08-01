#include "exti.h"

//KEY4外部中断控制蜂鸣器
//KEY4 ---> PC13  AHB1 
//SYSCFG----> APB2
void EXTI_KEY_Init()
{
	//1使能GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	//2.配置初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	//3.使能SYSCFG外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//4.配置EXTI线和IO对应关系
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
    //5.初始化EXTI配置
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line =   EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode =  EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//6.初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel =  EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd =  ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

#include "beep.h"
//7.编写中断服务函数
void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus( EXTI_Line13) == SET)
	{
		//BEEP = !BEEP;
	}

	EXTI_ClearITPendingBit(EXTI_Line13);
}
