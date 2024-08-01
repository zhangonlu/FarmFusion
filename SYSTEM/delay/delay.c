#include "delay.h"
///*-------------------------------------------
//函数:void delay_init(u8 SYSCLK)
//功能:滴答定时器初始化
//参数:
//	@SYSCLK:系统时钟(整数M):例如:168
//返回值:
//说明：
//	1.系统时钟 / 8 得到1秒所用的频率。
//		168MHz / 8 ==> 21MHz
//		则1秒跳动:21000000次
//	2.计算得到1微妙跳动的计数次数
//		21000000 / 1000/ 1000;
//		则1微妙跳动 21次
//	注意：计数器最大值(16777215次)
//---------------------------------------------*/
//u32 _us;	//1微秒所用节拍次数
//void delay_init(u8 SYSCLK)
//{
//	/*---- 1.设置系统时钟分频 ------*/
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//	/*---- 2.计算1微妙时间 ---------*/
//	_us = SYSCLK / 8;
//}
///*-------------------------------------------
//函数:void delay_us(u32 us)
//功能:微秒级延时
//参数:
//	@us:延时的微秒时间
//--------------------------------------------*/
//void delay_us(u32 us)
//{
//	u32 num = _us * us;				//计算us秒要跳动多少次
//	
//	SysTick->VAL = 	0;				//当前计数值为0;
//	SysTick->LOAD =	num;			//设置自动重装载值
//	SysTick->CTRL |= 0x1 << 0;		//使能开启滴答定时器
//		
//	while( ((SysTick->CTRL >> 16) & 0x1) == 0);	//死等滴答定时器向下计算到0，超时。
//	
//	SysTick->CTRL &= ~(0x1 << 0);	//使能关闭滴答定时器
//}
///*-------------------------------------------
//函数:void delay_us(u32 us)
//功能:毫秒级延时
//参数:
//	@us:延时的毫秒时间
//--------------------------------------------*/
//void delay_ms(u32 ms)
//{
//	while(ms--) delay_us(1000);
//}

//TIM7挂载在APB1总线 时钟频率为84MHZ
void TIM7_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    //时钟初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //1分频
    TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInitStruct.TIM_Period =  0xFFFF -1;         //自动重装载值
    TIM_TimeBaseInitStruct.TIM_Prescaler =  84-1;    //预分配值
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter  = 0; //高级定时器才开
	
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);

    //使能开启定时器
    TIM_Cmd(TIM7,ENABLE);
}

void delay_us(u32 us)
{
    u32 start_time = TIM7->CNT;
    u32 end_time = start_time + us;

    if(end_time > (0xffff-1))
    {
        while(TIM7->CNT < (0xffff - 1));
        end_time -= 0xffff - 1;
    }
	while(TIM7->CNT < end_time );

}

void delay_ms(u32 ms)
{
    while(ms--)
    {
        delay_us(1000);
    }

}




