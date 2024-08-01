#ifndef __IIC_H_
#define __IIC_H_


#include "sys.h"
#include "stdio.h"
//SCL¿≠µÕªÚ Õ∑≈
#define SCL_OUT PBOut(6)
#define SDA_OUT PBOut(7)



void IIC_RecvData(u8 Addr,u8 addr,u8 *Data,u8 len);

u8 IIC_ReadByte(void);
void IIC_SendByte(u8 ch);
u8 IIC_SendData(u8 Addr,u8 addr,u8 *Data,int len);
void MyIIC_Init(void);
void IIC_ACK(void);
#endif
