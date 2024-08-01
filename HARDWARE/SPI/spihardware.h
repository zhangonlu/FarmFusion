#ifndef __SPIHARDWARE_H_
#define __SPIHARDWARE_H_

#include "sys.h"

void SPI_Hard_Init(void);
void SPI_Hard_Start(void);
void SPI_Hard_Stop(void);
u8 SPI_Hard_SwapData(u8 data);
#endif