#ifndef __DMA_H_
#define __DMA_H_

#include "sys.h"

/**
���ܣ���������ת�ˣ������赽�洢����
@param1:Դ�˵�ַ
@param2:Ŀ�ĵ�ַ
@param3:�������
**/
void MYDMA_Init(u32 Addr1,u32 Addr2,u16 Size);
void Start_DMA(uint16_t Size);

#endif