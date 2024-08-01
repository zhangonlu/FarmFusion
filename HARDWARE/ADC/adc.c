#include "adc.h"
/*--------------------- ADC����Ĭ������ --------------------------
ADC_CommonInitStruct->ADC_Mode = ADC_Mode_Independent;						//ģʽ:����ģʽ
ADC_CommonInitStruct->ADC_Prescaler = ADC_Prescaler_Div2;					//��Ƶ:2��Ƶ , 84M / 2 ��Ϊ 42M 
ADC_CommonInitStruct->ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//DMA����:ʧ��
ADC_CommonInitStruct->ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//����ʱ��:5��ʱ������
---------------------- ADC����Ĭ������ --------------------------
ADC_InitStruct->ADC_Resolution = ADC_Resolution_12b;						//�ֱ���:12λ
ADC_InitStruct->ADC_ScanConvMode = DISABLE;									//ɨ�蹦��:ʧ��
ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;							//����ת����ʧ��
ADC_InitStruct->ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//������ʽ����ֹ
ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;			//������ʽ��
ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;						//���䷽ʽ���Ҷ���
ADC_InitStruct->ADC_NbrOfConversion = 1;									//�ɼ����������βɼ�
------------------------------------------------------------------*/
/*************************************************
������:void ADCx_Init(ADC_TypeDef* ADCx)
����:��ʼ��ADC
����:	
	ADCx��ADC��
����ֵ:��
˵��:
	ADC1��ʼ�� APB2 ����
**************************************************/
void ADCx_Init(ADC_TypeDef* ADCx)		//ADC��ʼ��
{
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.ʹ�� RCC_AHB1�������ߵ� GPIOA ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*----- 2.��ʼ��GPIO���� ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;					//ģʽ:ģ��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	//1.ʹ��ADC1 ����ʱ��
	if(ADCx == ADC1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	if(ADCx == ADC2)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	if(ADCx == ADC3)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	//2.����ADC�������� (�ܶ�ͨ�������õ���ͬһ��ADC)
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonStructInit(&ADC_CommonInitStruct);
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//3.ADC��������
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_Init(ADCx, &ADC_InitStruct);
	
	//4.ʹ��ADC����
	ADC_Cmd(ADCx, ENABLE);
}

u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch)
{
	//1.ָ��ADCͨ������:������ͨ��,һ�����в���ʱ�� ---- ����
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles);	
	
	//2.ָ��ADC����ת��
	ADC_SoftwareStartConv(ADCx);
	
	//3.�ȴ�EOC��0��ʾת���ɹ�
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) != SET);	//����ת���ɹ�
	
	//4.��ȡת�����������
	return ADC_GetConversionValue(ADCx);
}







