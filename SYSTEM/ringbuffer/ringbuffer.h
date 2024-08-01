#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include "sys.h"

#define RingBuffer_Length 128
typedef struct 
{
   u8 buffer[RingBuffer_Length]; //��������С
   int font;   //ͷ
   int rear;   //β
	u8 flag; 
}Ringbuffer_t;

//��ʼ�����λ�����
void RingBuffer_Init(Ringbuffer_t *ringbuffer);
//дһ���ֽڵ����λ�����
u8 RingBuffer_Write_Byte(Ringbuffer_t *ringbuffer ,u8 data);
//�ӻ��λ�������ȡһ���ֽ�
u8 RingBuffer_Read_Byte(Ringbuffer_t *ringbuffer );

//д��һƬ���ݵ�data ����д��״̬��0�ɹ� 1ʧ�ܣ�
u8  RingBuffer_Write_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len);

//����һƬ����data  ����д��״̬��0�ɹ�1ʧ�ܣ�
u8  RingBuffer_Read_Data(Ringbuffer_t *ringbuffer , u8 *data, u8  len);

u8  RingBuffer_GetLength(Ringbuffer_t *ringbuffer);
#endif