#include "spihardware.h"

//SPI1_MOSI PB5
//SPI1_MISO PB4
//SPI1_SCK  PB3
//W25Q_CS   PB2
//SPI1 APB2

//NSSƬѡ���Ų������ģ��IO 
#define NSS_OUT(state) GPIO_WriteBit(GPIOB,GPIO_Pin_2,state)


void SPI_Hard_Init()
{
       //1.IOʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    //2.CS����Ϊ������� 
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

	//MOSI  SCL����Ϊ����ģʽ 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 |GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);	

	//����Ƭѡ�ߣ��͵�ƽѡ�У�  
	NSS_OUT (1);

	//��ʼ��SPI����
	SPI_InitTypeDef SPI_InitStruct;
	//SPIģʽ���������Ǵӻ�
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	//����ʱ�ӷ�Ƶϵ��
	SPI_InitStruct.SPI_BaudRatePrescaler =  SPI_BaudRatePrescaler_128;
	//��һ�����ز��������룩
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	//ʱ�Ӽ��Գ�ʼ�͵�ƽ
	SPI_InitStruct.SPI_CPOL =  SPI_CPOL_Low;
	//
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	//ÿ�δ���8bit
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	//˫��ȫ˫������ģʽ
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//���ݸ�λ����
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
	//���ʵ��NSSƬѡ����
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
	SPI_Init(SPI1,&SPI_InitStruct);
	
	SPI_Cmd(SPI1,ENABLE);
}

void SPI_Hard_Start()
{
	NSS_OUT (0);
}

void SPI_Hard_Stop()
{
	NSS_OUT(1);
}



u8 SPI_Hard_SwapData(u8 data)
{
	//�ȴ�TXEλ��Ϊ1:��ʾTDR�����ͻ���Ĵ����������������������λ�Ĵ���
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	//����λ�Ĵ���������ݷ��ͳ�ȥ
	SPI_I2S_SendData(SPI1,data);
	
	//�ȴ�������ɣ�RXEλ�ᱻ��Ϊ1 ͬʱ�ӻ����������ݱ�����RDR�����ջ���Ĵ�����
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    //����RDR������� Ӳ���Զ����RXEλ
	return SPI_I2S_ReceiveData(SPI1);	
}
