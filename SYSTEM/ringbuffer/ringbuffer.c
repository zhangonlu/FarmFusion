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
	//1 �鿴�������Ƿ���
	if( (ringbuffer->rear + 1) % RingBuffer_Length  == ringbuffer->font)
		return 1;
	
	ringbuffer->buffer[ringbuffer->rear] = data;
	ringbuffer->rear = (ringbuffer->rear + 1) % RingBuffer_Length;
	return 0;
}
//�ӻ��λ�������ȡһ���ֽ�
u8 RingBuffer_Read_Byte(Ringbuffer_t *ringbuffer )
{
	//�жϻ��λ������Ƿ�Ϊ��
	if(ringbuffer->font == ringbuffer->rear)
		return 1;
	
	u8 data = ringbuffer->buffer[ringbuffer->font];
	ringbuffer->font =  (ringbuffer->font + 1) % RingBuffer_Length;
	return data;
}


#include "stdio.h"
//д��һƬ����
u8  RingBuffer_Write_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len)
{
	//�ж��ܷ�һ����д����ô������
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


//����һƬ����
u8  RingBuffer_Read_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len)
{
	//�ж��ܷ�һ���Զ�����ô������
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