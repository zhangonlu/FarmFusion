#include "pwm.h"
#include "led.h"

//呼吸灯 PE9 ----TIM1_ch1 
void PWM_Init()
{
	LED2 = 1;  
	//1.使能定时器1时钟和复用的IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	TIM_OCInitTypeDef TIM_OCInitStruct; 
	//2.配置GPIO口用于输出PWM信号 因为使用的是TIM1的CH1通道 对应PE9

	GPIO_InitTypeDef GPIO_InitStruct;
	 
	//配置为复用推挽模式 将引脚的控制权交给片上外设 PWM的波才能通过引脚输出 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);	
	 
	//3.初始化定时器配置
  	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//滤波器采样频率f的时钟分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//CNT计数器的计数模式
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//ARR自动重装载寄存器的值
	TIM_TimeBaseInitStruct.TIM_Period =  100 - 1;
	//PSC分频系数
	TIM_TimeBaseInitStruct.TIM_Prescaler =  840 - 1;
	//高级定时器的重复计数寄存器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//4.配置PWM输出通道

	//给结构体赋初值再修改需要的变量
	//TIM_OCStructInit(&TIM_OCInitStruct);
	//输出比较模式
	TIM_OCInitStruct.TIM_OCMode =  TIM_OCMode_PWM1;
	//输出比较极性 高极性：输出的REF波不翻转 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//输出使能
	TIM_OCInitStruct.TIM_OutputState =  TIM_OutputState_Enable;	
	//TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;

	//CCR寄存器的值
	TIM_OCInitStruct.TIM_Pulse = 100;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	

	//5.开启定时器
	TIM_Cmd(TIM1,ENABLE);
	//启用PWM的输出
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


//打开呼吸灯
u8 flg = 0;
#include "delay.h"
#include "freertos.h"
#include "task.h"
void LED_Toggri()
{  

	
	
   if(flg == 1)
   {
	 for(int i = 0;i < 100 && (flg == 1);i+=1)
	 {
		 vTaskDelay(20);
	   TIM_SetCompare1(TIM1,i); 
		 
	 }
	 for(int i = 100; i> 0&& (flg == 1);i-=1)
	 {
		  vTaskDelay(20);
	    TIM_SetCompare1(TIM1,i); 
	 }
   }
   else {
	  TIM_SetCompare1(TIM1,100); 
   }
   

	   

}



//定时器12的CH2 PB15
void PWM_Fs_Init()
{

	//1.使能定时器1时钟和复用的IO口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		TIM_OCInitTypeDef TIM_OCInitStruct; 
	GPIO_InitTypeDef GPIO_InitStruct;
	//配置为复用推挽模式 将引脚的控制权交给片上外设 PWM的波才能通过引脚输出 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);	

	
		//3.初始化定时器配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//滤波器采样频率f的时钟分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//CNT计数器的计数模式
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//ARR自动重装载寄存器的值
	TIM_TimeBaseInitStruct.TIM_Period =  100 - 1;
	//PSC分频系数   
	TIM_TimeBaseInitStruct.TIM_Prescaler =  8400 - 1;
	//高级定时器的重复计数寄存器
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStruct);
	
		//输出比较模式
	TIM_OCInitStruct.TIM_OCMode =  TIM_OCMode_PWM1;
	//输出比较极性 高极性：输出的REF波不翻转 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//输出使能
	TIM_OCInitStruct.TIM_OutputState =  TIM_OutputState_Enable;
		TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM12,&TIM_OCInitStruct);
		//5.开启定时器
	TIM_Cmd(TIM12,ENABLE);
	//启用PWM的输出
	TIM_CtrlPWMOutputs(TIM12,ENABLE);
}	

u8 fs_power = 0;
void FS_Controller()
{
	switch(fs_power)
	{
		case 0: TIM_SetCompare2(TIM12,0); break;
		case 1: TIM_SetCompare2(TIM12,35); break;
		case 2: TIM_SetCompare2(TIM12,70); break;
		case 3: TIM_SetCompare2(TIM12,100); break;
	}

}




