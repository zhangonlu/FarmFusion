#include "timer.h"

//TIM2 ---> APB1--->42MHz ��ʱ���豸�ᱶƵһ�� ---->84MHz


void TIMER_Init()
{
	//1ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//2.ѡ��ʱ����ʱ�ӣ���ѡ��Ĭ���ڲ�ʱ�ӣ�
	TIM_InternalClockConfig(TIM2);

	//3.��ʼ����ʱ������
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//�˲�������Ƶ��f��ʱ�ӷ�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
	//����������ģʽ������ ���� �������
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//�Զ���װ�ص�ֵ С��65535 1s(��ʱʱ��) = 84M / ����Ƶϵ�� * �Զ���װ��ֵ��
	TIM_TimeBaseInitStruct.TIM_Period =  10000 -1; 
	//Ԥ��Ƶ����Ƶϵ�� С��65535��2^16-1��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;
	//�ظ����������� ֻ�и߼���ʱ����
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	
	//�����ʼ�����µĸ����жϴ���
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	//4.ʹ�ܶ�ʱ���ж�
	 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	 
	//5.NVIC����
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	//6������ʱ��
	TIM_Cmd(TIM2, ENABLE);
}



//��ʱ���жϴ�����
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}


}

