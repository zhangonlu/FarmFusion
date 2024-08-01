#include "ringbuffer.h"
#include "string.h"

void RingBuffer_Init(Ringbuffer_t *ringbuffer)
{
	 
	memset(ringbuffer->buffer,0,sizeof(ringbuffer->buffer));
	ringbuffer->font = 0;
	ringbuffer->rear = 0;
	ringbuffer->flag = 0;
}

u8 RingBuffer_Write_Byte(Ringbuffer_t *ringbuffer , u8 data)
{
	//1 查看缓冲区是否满
	if( (ringbuffer->rear + 1) % RingBuffer_Length  == ringbuffer->font)
		return 1;
	
	ringbuffer->buffer[ringbuffer->rear] = data;
	ringbuffer->rear = (ringbuffer->rear + 1) % RingBuffer_Length;
	return 0;
}
//从环形缓冲区读取一个字节
u8 RingBuffer_Read_Byte(Ringbuffer_t *ringbuffer )
{
	//判断环形缓冲区是否为空
	if(ringbuffer->font == ringbuffer->rear)
		return 1;
	
	u8 data = ringbuffer->buffer[ringbuffer->font];
	ringbuffer->font =  (ringbuffer->font + 1) % RingBuffer_Length;
	return data;
}


#include "stdio.h"
//写入一片数据
u8  RingBuffer_Write_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len)
{
	//判断能否一次性写入这么多数据
	u8 length  = RingBuffer_GetLength(ringbuffer); 
	if(len + length> RingBuffer_Length)
	 {
		 printf("error: write multi data \r\n");
				 return 1;
	 }

	 for(int i = 0;i < len;i++)
	 {
		 RingBuffer_Write_Byte(ringbuffer ,data[i]);
	 }
	 
	 return 0;
}


//读出一片数据
u8  RingBuffer_Read_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len)
{
	//判断能否一次性读出这么多数据
	u8 length  = RingBuffer_GetLength(ringbuffer); 
	if(len  >  length)
	 {
		 printf("error: read multi data \r\n");
				 return 1;
	 }

	 for(int i = 0;i < len;i++)
	 {
		data[i] = RingBuffer_Read_Byte(ringbuffer);
		
	 }
	 
	 return 0;
}

u8  RingBuffer_GetLength(Ringbuffer_t *ringbuffer)
{
	return 	 ( RingBuffer_Length + ringbuffer->rear - ringbuffer->font ) % RingBuffer_Length;
}