#ifndef __DMA_H_
#define __DMA_H_

#include "sys.h"

/**
功能：进行数据转运（从外设到存储器）
@param1:源端地址
@param2:目的地址
@param3:传输次数
**/
void MYDMA_Init(u32 Addr1,u32 Addr2,u16 Size);
void Start_DMA(uint16_t Size);

#endif