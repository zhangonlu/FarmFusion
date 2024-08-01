#ifndef __ADCDMA_H_
#define __ADCDMA_H_

#include "sys.h"

extern uint16_t Data[2];
extern u16 light;
extern double vol;
void ADC_DMA_Init(void);
void DMA_Value(void);

#endif
