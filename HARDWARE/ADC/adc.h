#ifndef __ADC_H_
#define __ADC_H_

#include "sys.h"

//ADC单通道采集
void ADCx_Init(ADC_TypeDef* ADCx);
u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch);



#endif
