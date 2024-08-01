#include "adcdma.h"


//PA0 ---- ADC123IN_0 ---- ����ģ��
//PC0 ---- ADC123IN10 ---- ����������

uint16_t size; 

u16 DMA_Data[3];
void ADC_DMA_Init()
{
	//1.ʱ��ʹ��ADC GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
	
	//����GPIOΪģ��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//2.��ʼ����������ADC
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	//ʹ��DMAͨ��������ת�����
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler =  ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay =   ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//3.��ʼ��ADC����
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign =   ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1 ;
	ADC_InitStruct.ADC_ExternalTrigConvEdge =  ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion =   2;
	ADC_InitStruct.ADC_Resolution =   ADC_Resolution_12b ;
	ADC_InitStruct.ADC_ScanConvMode =        ENABLE;
	ADC_Init(ADC1,&ADC_InitStruct);
	//4.ADCͨ������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1 , ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,2 , ADC_SampleTime_144Cycles);
	//5.ADCʹ��
	ADC_Cmd(ADC1,ENABLE);
	
	//6.DMA����ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//7.DMA��ʼ������
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_Channel = DMA_Channel_0 ;
	DMA_InitStruct.DMA_BufferSize = 2;
	//���ݴ�������˵��ڴ�
	DMA_InitStruct.DMA_DIR =  DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable   ;
	DMA_InitStruct.DMA_FIFOThreshold =  DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)DMA_Data;
	DMA_InitStruct.DMA_MemoryBurst =  DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStruct.DMA_MemoryInc =  DMA_MemoryInc_Enable     ;
	DMA_InitStruct.DMA_Mode =   DMA_Mode_Circular           ;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_PeripheralBurst =   DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority =  DMA_Priority_High;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	
	DMA_Cmd(DMA2_Stream0,ENABLE);
	// ���� DMA ����ĺ���ת��
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}

u16 light = 0;
double vol = 0;
void DMA_Value()
{
	  light =  100 - ( DMA_Data[0] * 100 / 4096);
	
	   vol  = DMA_Data[1] * 3.3 / 4096;
		
	//��ѹС�����ֱ�����DAM_Data[2]
	
}