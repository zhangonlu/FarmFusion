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

    //2.MOSI CS SCL����Ϊ������� MISO����Ϊ�������루Ĭ�϶�ȡ���ߵ�ƽ��
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3 ;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//������:����
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

}


//������DHT11ͨ�Ź��̣�
//1.�ȷ����ź�ʱ�򣺰�������������ʼ�ź� �ӻ�������Ӧ�źţ�������ȡ�ӻ�����Ӧ�źţ�
//2.����ʱ�򣺴ӻ����͵ĸߵ͵�ƽ������һ��54us�ĵ͵�ƽ Ȼ��ͨ���ߵ�ƽ�ĳ���ʱ���ж�����Ϊ0����1
#include "delay.h"
void DHT11_Start()
{
	DATA_Model(1);
	DHT_DATA_OUT = 0;
	delay_ms(20);
	
	DHT_DATA_OUT = 1;
	delay_us(13);
}

//������ȡ�ӻ���Ӧ�ź� �õ���Ӧ����0
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
	
	//�ߵ�ƽ����ʱ�����50����λΪ1 ����50Ϊ0
	return time > 50 ? 1 : 0;
}

#define DHT11_IN PAIn(3)

u8 DHT11_Read_Bit(void)
{
	int timer = 0;
	/******1.��������ģʽ:���� *******/
	DATA_Model(0);
	
	/******2.����DHT11����,�ȴ�100us ***********/
	timer = 0;
	while(DHT11_IN == 1) //��������
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//��ʱ
	}
	
	/******3.������[50:58]us,����:54us,Ȼ������,�ȴ����ʱ��58us *****/
	while(DHT11_IN == 0) //��������
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//��ʱ
	}
	
	/******4.��¼�ߵ�ƽ����ʱ�� 0ֵ:[23-27]us, 1ֵ:[68-74] ����ѡ���ʱ��74us, *******/
	timer = 0;
	while(DHT11_IN == 1)	//��������
	{
		delay_us(1);
		timer++;
		if(timer > 100)  return 0;	//��ʱ
	}
	/******5.�жϸߵ�ƽ����ʱ�������ĸ���Χ 0ֵ:[23-27]us, 1ֵ:[68-74]***/
	return timer < 50 ? 0 : 1;
}

//DHT11�ᷢ��40bit������λ ��λ�ȳ�
//���ݸ�ʽ:8bitʪ����������+8bitʪ��С������+8bit�¶���������+8bit�¶�С������+8bitУ��͡�
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
//������������ 40bit  40/8 = 5
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

