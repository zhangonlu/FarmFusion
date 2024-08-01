#include "esp8266.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "led.h"
#include "pwm.h"
//WIFIʹ�ô���2��MCU����ͨ��

//USART2 -- APB1
//TCP���ջ�����
Ringbuffer_t ringbuffer;
//ͨ�û�����
Ringbuffer_t ringbuffer_Data;

BitAction flag_TXE = 0;

void  USART2_Init()
{
    //��ʼ�����λ�����
    RingBuffer_Init(&ringbuffer);
    RingBuffer_Init(&ringbuffer_Data);
    USART_InitTypeDef USART_InitStruct;	//���ڳ�ʼ���ṹ��
    NVIC_InitTypeDef NVIC_InitStruct;	//�жϳ�ʼ���ṹ��
    GPIO_InitTypeDef GPIO_InitStruct;	//GPIO��ʼ���ṹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /*---- 2.���Ÿ���ӳ�� ---*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    /*---- 3.��ʼ��GPIO���� ----*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			//ģʽ:����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	//����:5|6

    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*---- 4.UART��ʼ������ ----*/
    USART_InitStruct.USART_BaudRate = bound;										//������
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//�����ƣ���
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;						//ģʽ:��|��
    USART_InitStruct.USART_Parity = USART_Parity_No;								//У��λ
    USART_InitStruct.USART_StopBits = USART_StopBits_1;								//ֹͣλ
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;						//����λ

    USART_Init(USART2, &USART_InitStruct);

    /*---- 5.ʹ��UART���� -----*/
    USART_Cmd(USART2, ENABLE);

    /*---- 6.NVIC�ж����� -----*/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//��������ж�(�����ж�)

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;			//����1�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2

    NVIC_Init(&NVIC_InitStruct);
}


void USART2_IRQHandler()
{
    static u8 oldres,newres;

    //�������
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        oldres = newres;
        newres = USART_ReceiveData(USART2);
        USART_SendData(USART1,newres);

        RingBuffer_Write_Byte(&ringbuffer_Data , newres);
		RingBuffer_Write_Byte(&ringbuffer , newres);
		if(oldres == '*' && newres == '*')
        {
				ringbuffer.flag = 1;
		}
        //һ��������
        if(oldres == '\r' && newres == '\n')
        {

			
            char data[RingBuffer_Length+1];
            u8 len = RingBuffer_GetLength(&ringbuffer_Data);
            RingBuffer_Read_Data(&ringbuffer_Data , (u8 *)data,len );
            //���յ�OK
            if( (strncmp(data,"SEND OK",7) == 0))
            {
                ringbuffer_Data.flag = 2;
            }

            else if((strncmp(data,"OK",2) == 0) )
            {
                ringbuffer_Data.flag = 1;

            }

        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

}


//����2ʵ�ַ���һƬ����
void USART2_SendData(u8 *data,int len)
{
    for(int i = 0; i < len; i++)
    {
        //������һ�η������
        while((USART2->SR >> 6 & 0x01) == 0);
        USART_SendData(USART2,data[i]);
    }
}

//�ȴ�ESP8266����OK��־
u8 Wait_OK(u16 timer)
{
    while(timer > 0)
    {
        timer--;

        //д�����
        if(ringbuffer_Data.flag == 1)
        {
            ringbuffer_Data.flag = 0;
            return 0;
        }
        delay_ms(1);
    }

    return 1;
}


u8 Wait_SEND_OK(u16 timer)
{
    while(timer > 0)
    {
        timer--;

        //д�����
        if(ringbuffer_Data.flag == 2)
        {
            ringbuffer_Data.flag = 0;
            return 0;
        }
        delay_ms(1);
    }

    return 1;
}
//��Ƭ����ESP8266������Ϣ

u8 WIFI_SendData(char *data)
{
    USART2_SendData((u8 *)data,strlen((char *)data));

    return Wait_OK(time);
}

u8  ESP8266_MODEL2_Init()
{
    /*----- 1.����ģʽΪ AP�ȵ� ----*/
//	WIFI_SendData("AT+RST\r\n");
	    WIFI_SendData("AT+CWMODE=2\r\n");

    /*----- 2.�����ȵ� --------*/
    WIFI_SendData("AT+CWSAP=\"ZJJ\",\"88888888\",5,3\r\n");

    WIFI_SendData("AT+CIPMUX=1\r\n");
    WIFI_SendData("AT+CIPSERVER=1,10000\r\n");
}

u8  ESP8266_MODEL1_Init()
{
    //��Ҫ������������esp8266ģʽ
    WIFI_SendData("AT+RST\r\n");
    WIFI_SendData("AT+CWMODE=1\r\n");



}

void ESP8266_Close()
{
    WIFI_SendData("AT+RST\r\n");
}

u8 TCP_SendData(u8 *data,u8 uid,u32 len)
{
    //����ESP8266Ҫ����������
    char buf[128] = "";
    sprintf(buf,"AT+CIPSEND=%d,%d\r\n",uid,len);
    WIFI_SendData(buf);

    USART2_SendData(data,len);
    return Wait_SEND_OK(time);
}

u8 tcp_num = 0;
u8 tcp_num_flag = 0;
void TCP_Connected_Count()
{
    WIFI_SendData("AT+CIPSTATUS\r\n");
}









