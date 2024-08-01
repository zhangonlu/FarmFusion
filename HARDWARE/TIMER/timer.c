#include "timer.h"

//TIM2 ---> APB1--->42MHz 定时器设备会倍频一次 ---->84MHz


void TIMER_Init()
{
	//1使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//2.选择时基的时钟（不选则默认内部时钟）
	TIM_InternalClockConfig(TIM2);

	//3.初始化定时器配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//滤波器采样频率f的时钟分频系数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
	//计数器计数模式：向上 向下 中央对齐
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//自动重装载的值 小于65535 1s(定时时间) = 84M / （分频系数 * 自动重装载值）
	TIM_TimeBaseInitStruct.TIM_Period =  10000 -1; 
	//预分频器分频系数 小于65535（2^16-1）
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;
	//重复次数计数器 只有高级定时器有
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	
	//清除初始化导致的更新中断次数
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	//4.使能定时器中断
	 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	 
	//5.NVIC配置
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	//6开启定时器
	TIM_Cmd(TIM2, ENABLE);
}



//定时器中断处理函数
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}


}

