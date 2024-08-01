#include "iic.h"

//PB6 ���� SCL PB7----->SDA

//SDA = 1 �����ͷ�SDA�ߵĿ���Ȩ SDA = 0 ��������SDA
//SCL����ֻ�ܶ�ȡSDA �޷�����SDA  ֻ��SCL�͵�ƽ����²�������SDA��������ʼ�źź�ֹͣ�źţ�

//��ȡSDA��ƽ״̬
u8 SDA_READ()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
}

void MyIIC_Init(void)
{
    //1.ʹ��GPIOʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //2.��ʼ��GPIOB
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //3.���ó�ʼ״̬ �ߵ�ƽ
    GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);

}



#include "delay.h"
//void IIC_Delay()
//{
//    delay_us(1);
//}
void IIC_Delay()
{
	int i = 20;
    while(i--);
}

//IIC��ʼ�ź� SCL�ߵ�ƽ�ڼ�SDA����������
void IIC_Start()
{
    SCL_OUT = 1;
    SDA_OUT = 1;
    IIC_Delay();
    SDA_OUT = 0;
    IIC_Delay();

    SCL_OUT = 0;
}

//IICֹͣ�ź� SCL�ߵ�ƽ�ڼ� SDA����������
void IIC_Stop()
{
	
	SCL_OUT = 1;
    SDA_OUT = 0;

    IIC_Delay();

    SDA_OUT = 1;
    IIC_Delay();
}

//������Ϊ���ͷ�����һ���ֽ� ��λ���� eg:0xA0
//1010 0000 �ȷ���1
//1000 0000
void IIC_SendByte(u8 ch)
{
    //1��������SDA ����һ��bit
    for(int i = 0; i < 8; i++)
    {
        SDA_OUT =  (ch & 0x80) >> 7;
		ch <<= 1 ;

        IIC_Delay();

        //����SCL ���շ���ȡSDA
        SCL_OUT = 1;

        IIC_Delay();

        //��ȡ��Ϻ�����SCL �����´η���
        SCL_OUT = 0;
        IIC_Delay();
    }

}


//���շ���ȡһ���ֽ�����
// 0000 0000
// 1010 0000 �ȷ���1
//1000 0000  0x80
u8 IIC_ReadByte()
{
	//GPIOB->MODER &= ~(0x3 << 14);
    SDA_OUT = 1;
	u8 ch  = 0x00;
    for(int i = 0; i < 8; i++)
    {
		
        //���ͷ�����SCL �������ݶ�ȡ
        SCL_OUT = 1;
        ch |= SDA_READ() << (7-i);

        IIC_Delay();
        SCL_OUT = 0;
        IIC_Delay();

    }
	return ch;
}

//Ӧ���ź� SCL���� �ӻ�����������һ��Ӧ��λ SDA���ӻ�����
void IIC_ACK()
{
	//SCL�͵�ƽ�ڼ� ���������� ����Ӧ���ź�
	SDA_OUT = 0;
	
	//����SCL �ôӻ���ȡӦ���ź�
	SCL_OUT = 1;

    IIC_Delay();

    SCL_OUT = 0;
    IIC_Delay();
}

//��Ӧ���ź� SDAû�б��ӻ�����
void IIC_NACK()
{
    SCL_OUT = 1;
    SDA_OUT = 1;
    IIC_Delay();

    SCL_OUT = 0;
    IIC_Delay();
}


u8 Wait_ACK()
{
    //�����ͷ�SDA
    SDA_OUT = 1;
    IIC_Delay();
    //����SCL ��ȡSDA״̬
    SCL_OUT  = 1;
    IIC_Delay();

    //SDAû�б��ӻ�����
    if(SDA_READ() != 0)
    {
        IIC_Stop();
		printf("error\r\n");
        return 1;
    }


    SCL_OUT = 0;
    return 0;
}


//IIC����һ���ַ���
u8 IIC_SendData(u8 Addr,u8 addr,u8 *Data,int len)
{
    //1����������ʼ�ź�
    IIC_Start();

    //2�������ʹӻ��豸��ַ
    IIC_SendByte(Addr | 0x00);

    //3.�ӻ�����������Ӧ���ź� �����ȴ�Ӧ���ź�
    if(Wait_ACK() == 1)
        return 1;

    //4.��������Ҫ��д�ļĴ�����ַ
    IIC_SendByte(addr);
    if(Wait_ACK() == 1)
        return 1;

    //5.��������
    for(int i = 0; i < len; i++)
    {
        IIC_SendByte(Data[i]);
        //ÿ����һ�ֽ����ݶ�Ҫ�鿴�ӻ��Ƿ�Ӧ��
        if(Wait_ACK() == 1)
            return 1;
    }

    //6.���������� ��������ֹͣ�ź�
    IIC_Stop();

    return 0;
}

//����ָ����ַ��
void IIC_RecvData(u8 Addr,u8 addr,u8 *Data,u8 len)
{
    //1����������ʼ�ź�
    IIC_Start();

    //2����Ѱַ ���ö�д��ʽΪд
    IIC_SendByte(Addr | 0x00);
    if(Wait_ACK() == 1)
        return;

    //3�������ͼĴ�����ַ
    IIC_SendByte(addr);
    if(Wait_ACK() == 1)
        return;

    //4.���·�����ʼ�ź� �ı��д��ʽ
    IIC_Start();
    IIC_SendByte(Addr | 0x01);
    if(Wait_ACK() == 1)
        return;

    //5��ʼ��ȡ����
    for(int i = 0; i < len; i++)
    {
        Data[i] = IIC_ReadByte();
        //ÿ����һ���ֽ����� ��������Ӧ���ź�
        if(i < len - 1)
            IIC_ACK();
    }

    IIC_NACK();
    IIC_Stop();

}