#include "key.h"

//KEY1 ----> PE4
//KEY2 -->PE5
//KEY3---->PE6
//KEY4 ---->PC13
//AHB1

void KEY_Init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOE,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
    GPIO_Init(GPIOC,&GPIO_InitStruct);
}

#include "delay.h"

extern int key_state;
//按键扫描
u8 KEY_Scan()
{
    static u8 flag = 0;		//用于存储上次按键状态
	if((KEY1 == 0 || KEY2 == 0 || KEY3 == 0|| KEY4 == 0) && flag == 0)
	{
		flag = 1;		//按键按下保存
		delay_ms(10);	//延时消抖
		if(KEY1 == 0) return 1;
		if(KEY2 == 0) return 2;
		if(KEY3 == 0) return 3;
		if(KEY4 == 0) return 4;
	}
	
	if(KEY1 != 0 && KEY2 != 0 && KEY3 != 0&& KEY4 != 0)
	{
		flag = 0;		//按键没有按下保存
	}
	
	return 0;
}



