#include "led.h"
#include "delay.h"
//LED1 --- PE8 LED2 ---PE9 LED3 -- PE10
//GPIOE π“‘ÿ‘⁄AHB1◊‹œﬂ

void LED_Init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef GPIO_InitTypeDefStruct;
    GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitTypeDefStruct.GPIO_OType =  GPIO_OType_PP;
    GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);

    GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10);
}


u8 led1_state = 0;
u8 led3_state = 0;

LED_State LED_Status;

void LED_StatusHandle()
{
	LED_Status.Led1 = LED1;
	

}


