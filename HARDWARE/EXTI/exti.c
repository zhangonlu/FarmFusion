#include "exti.h"

//KEY4�ⲿ�жϿ��Ʒ�����
//KEY4 ---> PC13  AHB1 
//SYSCFG----> APB2
void EXTI_KEY_Init()
{
	//1ʹ��GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	//2.���ó�ʼ��GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	//3.ʹ��SYSCFG����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//4.����EXTI�ߺ�IO��Ӧ��ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
    //5.��ʼ��EXTI����
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line =   EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode =  EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//6.��ʼ��NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel =  EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd =  ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

#include "beep.h"
//7.��д�жϷ�����
void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus( EXTI_Line13) == SET)
	{
		//BEEP = !BEEP;
	}

	EXTI_ClearITPendingBit(EXTI_Line13);
}
