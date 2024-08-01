#include "pwm.h"
#include "led.h"

//������ PE9 ----TIM1_ch1 
void PWM_Init()
{
	LED2 = 1;  
	//1.ʹ�ܶ�ʱ��1ʱ�Ӻ͸��õ�IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	TIM_OCInitTypeDef TIM_OCInitStruct; 
	//2.����GPIO���������PWM�ź� ��Ϊʹ�õ���TIM1��CH1ͨ�� ��ӦPE9

	GPIO_InitTypeDef GPIO_InitStruct;
	 
	//����Ϊ��������ģʽ �����ŵĿ���Ȩ����Ƭ������ PWM�Ĳ�����ͨ��������� 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);	
	 
	//3.��ʼ����ʱ������
  	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//�˲�������Ƶ��f��ʱ�ӷ�Ƶ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//CNT�������ļ���ģʽ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//ARR�Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseInitStruct.TIM_Period =  100 - 1;
	//PSC��Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_Prescaler =  840 - 1;
	//�߼���ʱ�����ظ������Ĵ���
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//4.����PWM���ͨ��

	//���ṹ�帳��ֵ���޸���Ҫ�ı���
	//TIM_OCStructInit(&TIM_OCInitStruct);
	//����Ƚ�ģʽ
	TIM_OCInitStruct.TIM_OCMode =  TIM_OCMode_PWM1;
	//����Ƚϼ��� �߼��ԣ������REF������ת 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//���ʹ��
	TIM_OCInitStruct.TIM_OutputState =  TIM_OutputState_Enable;	
	//TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;

	//CCR�Ĵ�����ֵ
	TIM_OCInitStruct.TIM_Pulse = 100;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	

	//5.������ʱ��
	TIM_Cmd(TIM1,ENABLE);
	//����PWM�����
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


//�򿪺�����
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



//��ʱ��12��CH2 PB15
void PWM_Fs_Init()
{

	//1.ʹ�ܶ�ʱ��1ʱ�Ӻ͸��õ�IO��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		TIM_OCInitTypeDef TIM_OCInitStruct; 
	GPIO_InitTypeDef GPIO_InitStruct;
	//����Ϊ��������ģʽ �����ŵĿ���Ȩ����Ƭ������ PWM�Ĳ�����ͨ��������� 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);	

	
		//3.��ʼ����ʱ������
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//�˲�������Ƶ��f��ʱ�ӷ�Ƶ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//CNT�������ļ���ģʽ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//ARR�Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseInitStruct.TIM_Period =  100 - 1;
	//PSC��Ƶϵ��   
	TIM_TimeBaseInitStruct.TIM_Prescaler =  8400 - 1;
	//�߼���ʱ�����ظ������Ĵ���
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStruct);
	
		//����Ƚ�ģʽ
	TIM_OCInitStruct.TIM_OCMode =  TIM_OCMode_PWM1;
	//����Ƚϼ��� �߼��ԣ������REF������ת 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//���ʹ��
	TIM_OCInitStruct.TIM_OutputState =  TIM_OutputState_Enable;
		TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM12,&TIM_OCInitStruct);
		//5.������ʱ��
	TIM_Cmd(TIM12,ENABLE);
	//����PWM�����
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




