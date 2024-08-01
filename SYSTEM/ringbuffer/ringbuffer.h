#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include "sys.h"

#define RingBuffer_Length 128
typedef struct 
{
   u8 buffer[RingBuffer_Length]; //缓冲区大小
   int font;   //头
   int rear;   //尾
	u8 flag; 
}Ringbuffer_t;

//初始化环形缓冲区
void RingBuffer_Init(Ringbuffer_t *ringbuffer);
//写一个字节到环形缓冲区
u8 RingBuffer_Write_Byte(Ringbuffer_t *ringbuffer ,u8 data);
//从环形缓冲区读取一个字节
u8 RingBuffer_Read_Byte(Ringbuffer_t *ringbuffer );

//写入一片数据到data 返回写入状态（0成功 1失败）
u8  RingBuffer_Write_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len);

//读出一片数据data  返回写入状态（0成功1失败）
u8  RingBuffer_Read_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len);

u8  RingBuffer_GetLength(Ringbuffer_t *ringbuffer);
#endif