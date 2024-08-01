#ifndef __WIFI_H_
#define __WIFI_H_

#include "sys.h"
#include "ringbuffer.h"

#define bound 115200
#define time 500
//MCU��ESP8266�������ݵĻ����� 0x5555��ͷ 0xFFFF��β
extern Ringbuffer_t ringbuffer;

//ESP8266���͸����ڵ�����
extern Ringbuffer_t ringbuffer_Data;


extern u8 tcp_num_flag;
extern u8 tcp_num ;

void  USART2_Init(void);
u8 WIFI_SendData(char *data);
void ESP8266_Close(void);
u8  ESP8266_MODEL2_Init(void);
u8  ESP8266_MODEL1_Init(void);
u8 TCP_SendData(u8 *data,u8 uid,u32 len);
void TCP_Connected_Count(void);


#endif