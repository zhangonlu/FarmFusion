#include "iic.h"

//PB6 —— SCL PB7----->SDA

//SDA = 1 主机释放SDA线的控制权 SDA = 0 主机拉低SDA
//SCL拉高只能读取SDA 无法拉低SDA  只有SCL低电平情况下才能拉低SDA（除了起始信号和停止信号）

//读取SDA电平状态
u8 SDA_READ()
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
}

void MyIIC_Init(void)
{
    //1.使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //2.初始化GPIOB
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //3.设置初始状态 高电平
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

//IIC起始信号 SCL高电平期间SDA被主机拉低
void IIC_Start()
{
    SCL_OUT = 1;
    SDA_OUT = 1;
    IIC_Delay();
    SDA_OUT = 0;
    IIC_Delay();

    SCL_OUT = 0;
}

//IIC停止信号 SCL高电平期间 SDA被主机拉高
void IIC_Stop()
{
	
	SCL_OUT = 1;
    SDA_OUT = 0;

    IIC_Delay();

    SDA_OUT = 1;
    IIC_Delay();
}

//主机作为发送方发送一个字节 高位先行 eg:0xA0
//1010 0000 先发送1
//1000 0000
void IIC_SendByte(u8 ch)
{
    //1主机拉低SDA 发送一个bit
    for(int i = 0; i < 8; i++)
    {
        SDA_OUT =  (ch & 0x80) >> 7;
		ch <<= 1 ;

        IIC_Delay();

        //拉高SCL 接收方读取SDA
        SCL_OUT = 1;

        IIC_Delay();

        //读取完毕后拉低SCL 方便下次发送
        SCL_OUT = 0;
        IIC_Delay();
    }

}


//接收方读取一个字节数据
// 0000 0000
// 1010 0000 先发送1
//1000 0000  0x80
u8 IIC_ReadByte()
{
	//GPIOB->MODER &= ~(0x3 << 14);
    SDA_OUT = 1;
	u8 ch  = 0x00;
    for(int i = 0; i < 8; i++)
    {
		
        //发送方拉高SCL 进行数据读取
        SCL_OUT = 1;
        ch |= SDA_READ() << (7-i);

        IIC_Delay();
        SCL_OUT = 0;
        IIC_Delay();

    }
	return ch;
}

//应答信号 SCL拉高 从机给主机发送一个应答位 SDA被从机拉低
void IIC_ACK()
{
	//SCL低电平期间 拉低数据线 产生应答信号
	SDA_OUT = 0;
	
	//拉高SCL 让从机读取应答信号
	SCL_OUT = 1;

    IIC_Delay();

    SCL_OUT = 0;
    IIC_Delay();
}

//非应答信号 SDA没有被从机拉低
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
    //主机释放SDA
    SDA_OUT = 1;
    IIC_Delay();
    //拉高SCL 读取SDA状态
    SCL_OUT  = 1;
    IIC_Delay();

    //SDA没有被从机拉低
    if(SDA_READ() != 0)
    {
        IIC_Stop();
		printf("error\r\n");
        return 1;
    }


    SCL_OUT = 0;
    return 0;
}


//IIC发送一个字符串
u8 IIC_SendData(u8 Addr,u8 addr,u8 *Data,int len)
{
    //1主机发送起始信号
    IIC_Start();

    //2主机发送从机设备地址
    IIC_SendByte(Addr | 0x00);

    //3.从机向主机发送应答信号 主机等待应答信号
    if(Wait_ACK() == 1)
        return 1;

    //4.主机发送要读写的寄存器地址
    IIC_SendByte(addr);
    if(Wait_ACK() == 1)
        return 1;

    //5.发送数据
    for(int i = 0; i < len; i++)
    {
        IIC_SendByte(Data[i]);
        //每发送一字节数据都要查看从机是否应答
        if(Wait_ACK() == 1)
            return 1;
    }

    //6.发送完数据 主机发送停止信号
    IIC_Stop();

    return 0;
}

//主机指定地址读
void IIC_RecvData(u8 Addr,u8 addr,u8 *Data,u8 len)
{
    //1主机发送起始信号
    IIC_Start();

    //2主机寻址 设置读写方式为写
    IIC_SendByte(Addr | 0x00);
    if(Wait_ACK() == 1)
        return;

    //3主机发送寄存器地址
    IIC_SendByte(addr);
    if(Wait_ACK() == 1)
        return;

    //4.重新发送起始信号 改变读写方式
    IIC_Start();
    IIC_SendByte(Addr | 0x01);
    if(Wait_ACK() == 1)
        return;

    //5开始读取数据
    for(int i = 0; i < len; i++)
    {
        Data[i] = IIC_ReadByte();
        //每接收一个字节数据 主机发送应答信号
        if(i < len - 1)
            IIC_ACK();
    }

    IIC_NACK();
    IIC_Stop();

}
