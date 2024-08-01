#include "esp8266.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "led.h"
#include "pwm.h"
//WIFI使用串口2和MCU进行通信

//USART2 -- APB1
//TCP接收缓冲区
Ringbuffer_t ringbuffer;
//通用缓冲区
Ringbuffer_t ringbuffer_Data;

BitAction flag_TXE = 0;

void  USART2_Init()
{
    //初始化环形缓冲区
    RingBuffer_Init(&ringbuffer);
    RingBuffer_Init(&ringbuffer_Data);
    USART_InitTypeDef USART_InitStruct;	//串口初始化结构体
    NVIC_InitTypeDef NVIC_InitStruct;	//中断初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct;	//GPIO初始化结构体
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /*---- 2.引脚复用映射 ---*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    /*---- 3.初始化GPIO引脚 ----*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			//模式:复用
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	//引脚:5|6

    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*---- 4.UART初始化设置 ----*/
    USART_InitStruct.USART_BaudRate = bound;										//波特率
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//流控制：无
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;						//模式:收|发
    USART_InitStruct.USART_Parity = USART_Parity_No;								//校验位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;								//停止位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;						//数据位

    USART_Init(USART2, &USART_InitStruct);

    /*---- 5.使能UART串口 -----*/
    USART_Cmd(USART2, ENABLE);

    /*---- 6.NVIC中断配置 -----*/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启相关中断(接收中断)

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;			//串口1中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;			//响应优先级2

    NVIC_Init(&NVIC_InitStruct);
}


void USART2_IRQHandler()
{
    static u8 oldres,newres;

    //接收完成
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
        //一句接收完成
        if(oldres == '\r' && newres == '\n')
        {

			
            char data[RingBuffer_Length+1];
            u8 len = RingBuffer_GetLength(&ringbuffer_Data);
            RingBuffer_Read_Data(&ringbuffer_Data , (u8 *)data,len );
            //接收到OK
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


//串口2实现发送一片数据
void USART2_SendData(u8 *data,int len)
{
    for(int i = 0; i < len; i++)
    {
        //死等上一次发送完成
        while((USART2->SR >> 6 & 0x01) == 0);
        USART_SendData(USART2,data[i]);
    }
}

//等待ESP8266发送OK标志
u8 Wait_OK(u16 timer)
{
    while(timer > 0)
    {
        timer--;

        //写入完成
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

        //写入完成
        if(ringbuffer_Data.flag == 2)
        {
            ringbuffer_Data.flag = 0;
            return 0;
        }
        delay_ms(1);
    }

    return 1;
}
//单片机给ESP8266发送消息

u8 WIFI_SendData(char *data)
{
    USART2_SendData((u8 *)data,strlen((char *)data));

    return Wait_OK(time);
}

u8  ESP8266_MODEL2_Init()
{
    /*----- 1.设置模式为 AP热点 ----*/
//	WIFI_SendData("AT+RST\r\n");
	    WIFI_SendData("AT+CWMODE=2\r\n");

    /*----- 2.配置热点 --------*/
    WIFI_SendData("AT+CWSAP=\"ZJJ\",\"88888888\",5,3\r\n");

    WIFI_SendData("AT+CIPMUX=1\r\n");
    WIFI_SendData("AT+CIPSERVER=1,10000\r\n");
}

u8  ESP8266_MODEL1_Init()
{
    //需要重启才能配置esp8266模式
    WIFI_SendData("AT+RST\r\n");
    WIFI_SendData("AT+CWMODE=1\r\n");



}

void ESP8266_Close()
{
    WIFI_SendData("AT+RST\r\n");
}

u8 TCP_SendData(u8 *data,u8 uid,u32 len)
{
    //告诉ESP8266要发多少数据
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









