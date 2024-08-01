#include "usart1.h"

//USART1_TX ----> PA9
//USART1_RX-----> PA10
//USART1--->APB2
void USART1_Init()
{
    //1.ʹ��GPIO	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	
	//2.��ʼ��GPIOA
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.ʹ��USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//4.����GPIOA9��10ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//5.��ʼ������USART1
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = bound;
	USART_InitStruct.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity =  USART_Parity_No;
	USART_InitStruct.USART_StopBits =   USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  
	USART_Init(USART1, &USART_InitStruct);
	
	//ʹ��USART1����
	USART_Cmd(USART1, ENABLE);
	//���������ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//6.NVIC��ʼ������
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

}

void USART1_IRQHandler()
{
	u8 ret;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		 ret = USART_ReceiveData(USART1);
		 USART_SendData(USART2,ret);	
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

#include "stdio.h"
int fputc(int ch ,FILE *f)
{
     //USART��SR�Ĵ��7λ��1��ʾ������� 0x40 ---> 0x 0100 0000
	 while( (USART1->SR & 0x40) == 0);
	 //DR�Ĵ����������
	 USART1->DR = (u8)ch;
	 return ch;
}
