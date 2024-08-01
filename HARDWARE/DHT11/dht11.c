#include "dht11.h"


void DATA_Model(u8 model)
{
	if(model == 0)
       GPIOA->MODER &= ~(0x03 << 6);
	else   GPIOA->MODER |= (0x01 << 6);
}


void DHT11_Init()
{
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //2.MOSI CS SCL配置为推挽输出 MISO配置为上拉输入（默认读取到高电平）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3 ;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//上下拉:上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

}


//主机和DHT11通信过程：
//1.先发送信号时序：包括主机发送起始信号 从机发送响应信号（主机读取从机的响应信号）
//2.数据时序：从机发送的高低电平都是先一个54us的低电平 然后通过高电平的持续时间判断数据为0还是1
#include "delay.h"
void DHT11_Start()
{
	DATA_Model(1);
	DHT_DATA_OUT = 0;
	delay_ms(20);
	
	DHT_DATA_OUT = 1;
	delay_us(13);
}

//主机读取从机响应信号 得到响应返回0
u8 DHT11_Check()
{
	DATA_Model(0);
	u8 time = 0;	
	while(DHT_DATA_IN == 1)
	{
		delay_us(1);
		time++;
		if(time > 100) return 1;
	}
    
	time = 0;
	while(DHT_DATA_IN == 0)
	{
		delay_us(1);
		time++;
		if(time > 88) return 1;
	}
	
	
	delay_us(87);
	return 0;
}

u8 DHT_ReadBit(void)
{
	DATA_Model(0);
	u8 time = 0;
	while(DHT_DATA_IN == 1)
	{
		delay_us(1);
		time++;
		if(time > 100) return 0;
	}

	//time = 0;
	while(DHT_DATA_IN == 0)
	{
		delay_us(1);
		time++;
		if(time > 100) return 0;
	}
	time = 0;
	while(DHT_DATA_IN == 1)
	{
		delay_us(1);
		time++;
		if(time > 100) return 0;
	}
	
	//高电平持续时间大于50数据位为1 低于50为0
	return time > 50 ? 1 : 0;
}

#define DHT11_IN PAIn(3)

u8 DHT11_Read_Bit(void)
{
	int timer = 0;
	/******1.设置引脚模式:输入 *******/
	DATA_Model(0);
	
	/******2.死等DHT11拉低,等待100us ***********/
	timer = 0;
	while(DHT11_IN == 1) //死等拉低
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//超时
	}
	
	/******3.会拉低[50:58]us,经典:54us,然后拉高,等待最大时间58us *****/
	while(DHT11_IN == 0) //死等拉高
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//超时
	}
	
	/******4.记录高电平持续时间 0值:[23-27]us, 1值:[68-74] 我们选择最长时间74us, *******/
	timer = 0;
	while(DHT11_IN == 1)	//死等拉低
	{
		delay_us(1);
		timer++;
		if(timer > 100)  return 0;	//超时
	}
	/******5.判断高电平持续时间属于哪个范围 0值:[23-27]us, 1值:[68-74]***/
	return timer < 50 ? 0 : 1;
}

//DHT11会发送40bit的数据位 高位先出
//数据格式:8bit湿度整数数据+8bit湿度小数数据+8bit温度整数数据+8bit温度小数数据+8bit校验和。
//tem: 1010 1100
//res: 0000 0000
u8 DHT_ReadByte()
{
	u8 res = 0;
   	for(int i = 0;i < 8;i++)
	{
		res <<= 1;
		res |= DHT11_Read_Bit();
	}
	

	return res;
}

#include "stdio.h"
DHT11_t dht11_data;
//完整读出数据 40bit  40/8 = 5
u8 DHT_ReadData(DHT11_t *dht11)
{
	DHT11_Init();
	DHT11_Start();
	u8 data[5] = {0};
	if( DHT11_Check() != 0)
	{
		return 1;
	}
	for(int i = 0;i < 5;i++)
    { 	
	   data[i] = DHT_ReadByte();	
	}
	
	if(data[0] + data[1] + data[2] + data[3] == data[4])
	{
		dht11->Hum_H = data[0];
		dht11->Hum_L = data[1];
		dht11->Tem_H = data[2];
		dht11->Tem_L = data[3];
		dht11->Hum = data[0] +  data[1]*0.01;
		dht11->Tem = data[2] +  data[3]*0.01;
		
	}
	
	return 0;
}

