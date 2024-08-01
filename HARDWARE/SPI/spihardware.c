#include "spihardware.h"

//SPI1_MOSI PB5
//SPI1_MISO PB4
//SPI1_SCK  PB3
//W25Q_CS   PB2
//SPI1 APB2

//NSS片选引脚采用软件模拟IO 
#define NSS_OUT(state) GPIO_WriteBit(GPIOB,GPIO_Pin_2,state)


void SPI_Hard_Init()
{
       //1.IO时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    //2.CS配置为推挽输出 
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

	//MOSI  SCL配置为复用模式 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 |GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);	

	//拉高片选线（低电平选中）  
	NSS_OUT (1);

	//初始化SPI配置
	SPI_InitTypeDef SPI_InitStruct;
	//SPI模式：主机还是从机
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	//设置时钟分频系数
	SPI_InitStruct.SPI_BaudRatePrescaler =  SPI_BaudRatePrescaler_128;
	//第一个边沿采样（移入）
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	//时钟极性初始低电平
	SPI_InitStruct.SPI_CPOL =  SPI_CPOL_Low;
	//
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	//每次传输8bit
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	//双线全双工传输模式
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//数据高位先行
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
	//软件实现NSS片选引脚
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
	//等待TXE位置为1:表示TDR（发送缓冲寄存器）里的数据整体移向移位寄存器
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	//将移位寄存器里的数据发送出去
	SPI_I2S_SendData(SPI1,data);
	
	//等待发送完成，RXE位会被置为1 同时从机发来的数据保存在RDR（接收缓冲寄存器）
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    //读走RDR里的数据 硬件自动清除RXE位
	return SPI_I2S_ReceiveData(SPI1);	
}
