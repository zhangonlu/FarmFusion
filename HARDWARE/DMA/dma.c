#include "dma.h"
//DMA数据搬运 
//1.外设和存储器（RAM）之间数据转运
//2.存储器之间 ROM 和 RAM   F4/F7只有DMA2可以在存储器之间搬运数据

void MYDMA_Init(u32 Addr1,u32 Addr2,u16 Size)
{
	//1使能DMA外设时钟 AHB1总线
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

//	//2.DMA配置
    DMA_InitTypeDef DMA_InitStruct;
	    // 失能DMA流，在重新配置时必须先失能
    DMA_Cmd(DMA2_Stream0, DISABLE);

    // 等待流失能完成
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	//转运次数   自动重装计数器
	DMA_InitStruct.DMA_BufferSize =  Size;
	//DMA流的通道选择  F4两个DAMA 每个16个流 每个流8个通道
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;
	//传输方向：从外设到内存	
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;
	//指定流的模式是否为FIFO（先进先出）
	DMA_InitStruct.DMA_FIFOMode =  DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold =  DMA_FIFOThreshold_1QuarterFull;
	//存储器的地址
	DMA_InitStruct.DMA_Memory0BaseAddr = Addr2;
	 /*	当 AHB 外设端口被配置为单次传输时，根据 DMA_SxCR 寄存器 PSIZE[1:0] 位的值， 
	每个 DMA 请求产生一次字节、半字或字的数据传输。
	当 AHB 外设端口被配置为突发传输时，根据 DMA_SxCR 寄存器 PBURST[1:0]
	和PSIZE[1:0] 位的值，每个 DMA 请求相应地生成 4 个、8 个或 16 个节拍的字节、半字或字的传输。 */
	DMA_InitStruct.DMA_MemoryBurst =  DMA_MemoryBurst_Single;
	//数据宽度 8 16 32位 尽量保存源端和目的端数据宽度一致
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//指定内存地址是否自增
	DMA_InitStruct.DMA_MemoryInc =  DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode =  DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr =  Addr1;
	//每次传输一个字节
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	//指定外设地址是否自增
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	
	DMA_InitStruct.DMA_Priority =  DMA_Priority_Medium ;
    DMA_Init(DMA2_Stream0,&DMA_InitStruct);

	//默认初始化结构体
	//DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, DISABLE);
}


void Start_DMA(uint16_t Size)
{
	//失能DMA
   DMA_Cmd(DMA2_Stream0, DISABLE);
	    // 等待DMA失能完成
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	//设置重装计数器的值
   DMA_SetCurrDataCounter(DMA2_Stream0, Size);

	//使能DMA
	 DMA_Cmd(DMA2_Stream0, ENABLE);
	//等待DMA转运完成
   while(DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0) == RESET);	
//清除转运完成标志位	
   DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);

}


