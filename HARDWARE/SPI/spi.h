#ifndef __SPI_H_
#define __SPI_H_

#include "sys.h"



#define MOSI_OUT PBOut(5)
#define CS_OUT   PBOut(2)
#define SCL_OUT  PBOut(3)
#define MISO_IN  PBIn(4)
	

//#define MOSI_OUT(state)   GPIO_WriteBit(GPIOB,GPIO_Pin_5,state)
//#define CS_OUT(state)    GPIO_WriteBit(GPIOB,GPIO_Pin_2,state)
//#define SCL_OUT(state)     GPIO_WriteBit(GPIOB,GPIO_Pin_3,state)
     
uint8_t SPI_SendByte(uint8_t Data);
void    SPI_Start(void);
void    SPI_SoftWare_AF_Init(void);
void    SPI_Stop(void);
uint8_t SPI_SendByte_2(uint8_t Data);
#endif