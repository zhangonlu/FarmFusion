#include "delay.h"
///*-------------------------------------------
//����:void delay_init(u8 SYSCLK)
//����:�δ�ʱ����ʼ��
//����:
//	@SYSCLK:ϵͳʱ��(����M):����:168
//����ֵ:
//˵����
//	1.ϵͳʱ�� / 8 �õ�1�����õ�Ƶ�ʡ�
//		168MHz / 8 ==> 21MHz
//		��1������:21000000��
//	2.����õ�1΢�������ļ�������
//		21000000 / 1000/ 1000;
//		��1΢������ 21��
//	ע�⣺���������ֵ(16777215��)
//---------------------------------------------*/
//u32 _us;	//1΢�����ý��Ĵ���
//void delay_init(u8 SYSCLK)
//{
//	/*---- 1.����ϵͳʱ�ӷ�Ƶ ------*/
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//	/*---- 2.����1΢��ʱ�� ---------*/
//	_us = SYSCLK / 8;
//}
///*-------------------------------------------
//����:void delay_us(u32 us)
//����:΢�뼶��ʱ
//����:
//	@us:��ʱ��΢��ʱ��
//--------------------------------------------*/
//void delay_us(u32 us)
//{
//	u32 num = _us * us;				//����us��Ҫ�������ٴ�
//	
//	SysTick->VAL = 	0;				//��ǰ����ֵΪ0;
//	SysTick->LOAD =	num;			//�����Զ���װ��ֵ
//	SysTick->CTRL |= 0x1 << 0;		//ʹ�ܿ����δ�ʱ��
//		
//	while( ((SysTick->CTRL >> 16) & 0x1) == 0);	//���ȵδ�ʱ�����¼��㵽0����ʱ��
//	
//	SysTick->CTRL &= ~(0x1 << 0);	//ʹ�ܹرյδ�ʱ��
//}
///*-------------------------------------------
//����:void delay_us(u32 us)
//����:���뼶��ʱ
//����:
//	@us:��ʱ�ĺ���ʱ��
//--------------------------------------------*/
//void delay_ms(u32 ms)
//{
//	while(ms--) delay_us(1000);
//}

//TIM7������APB1���� ʱ��Ƶ��Ϊ84MHZ
void TIM7_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    //ʱ�ӳ�ʼ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //1��Ƶ
    TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up; //���ϼ���
    TIM_TimeBaseInitStruct.TIM_Period =  0xFFFF -1;         //�Զ���װ��ֵ
    TIM_TimeBaseInitStruct.TIM_Prescaler =  84-1;    //Ԥ����ֵ
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter  = 0; //�߼���ʱ���ſ�
	
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);

    //ʹ�ܿ�����ʱ��
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




