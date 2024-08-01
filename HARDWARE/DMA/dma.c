#include "dma.h"
//DMA���ݰ��� 
//1.����ʹ洢����RAM��֮������ת��
//2.�洢��֮�� ROM �� RAM   F4/F7ֻ��DMA2�����ڴ洢��֮���������

void MYDMA_Init(u32 Addr1,u32 Addr2,u16 Size)
{
	//1ʹ��DMA����ʱ�� AHB1����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

//	//2.DMA����
    DMA_InitTypeDef DMA_InitStruct;
	    // ʧ��DMA��������������ʱ������ʧ��
    DMA_Cmd(DMA2_Stream0, DISABLE);

    // �ȴ���ʧ�����
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	//ת�˴���   �Զ���װ������
	DMA_InitStruct.DMA_BufferSize =  Size;
	//DMA����ͨ��ѡ��  F4����DAMA ÿ��16���� ÿ����8��ͨ��
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;
	//���䷽�򣺴����赽�ڴ�	
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToMemory;
	//ָ������ģʽ�Ƿ�ΪFIFO���Ƚ��ȳ���
	DMA_InitStruct.DMA_FIFOMode =  DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold =  DMA_FIFOThreshold_1QuarterFull;
	//�洢���ĵ�ַ
	DMA_InitStruct.DMA_Memory0BaseAddr = Addr2;
	 /*	�� AHB ����˿ڱ�����Ϊ���δ���ʱ������ DMA_SxCR �Ĵ��� PSIZE[1:0] λ��ֵ�� 
	ÿ�� DMA �������һ���ֽڡ����ֻ��ֵ����ݴ��䡣
	�� AHB ����˿ڱ�����Ϊͻ������ʱ������ DMA_SxCR �Ĵ��� PBURST[1:0]
	��PSIZE[1:0] λ��ֵ��ÿ�� DMA ������Ӧ������ 4 ����8 ���� 16 �����ĵ��ֽڡ����ֻ��ֵĴ��䡣 */
	DMA_InitStruct.DMA_MemoryBurst =  DMA_MemoryBurst_Single;
	//���ݿ�� 8 16 32λ ��������Դ�˺�Ŀ�Ķ����ݿ��һ��
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//ָ���ڴ��ַ�Ƿ�����
	DMA_InitStruct.DMA_MemoryInc =  DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode =  DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr =  Addr1;
	//ÿ�δ���һ���ֽ�
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	//ָ�������ַ�Ƿ�����
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	
	DMA_InitStruct.DMA_Priority =  DMA_Priority_Medium ;
    DMA_Init(DMA2_Stream0,&DMA_InitStruct);

	//Ĭ�ϳ�ʼ���ṹ��
	//DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, DISABLE);
}


void Start_DMA(uint16_t Size)
{
	//ʧ��DMA
   DMA_Cmd(DMA2_Stream0, DISABLE);
	    // �ȴ�DMAʧ�����
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	//������װ��������ֵ
   DMA_SetCurrDataCounter(DMA2_Stream0, Size);

	//ʹ��DMA
	 DMA_Cmd(DMA2_Stream0, ENABLE);
	//�ȴ�DMAת�����
   while(DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0) == RESET);	
//���ת����ɱ�־λ	
   DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);

}


