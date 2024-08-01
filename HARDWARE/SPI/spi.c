#include "spi.h"


//软件模拟SPI
//主机MOSI-->PB5
//MISO ---> PB4
//SCL ----> PB3
//W25Q CS ---> PB2

//u8 MISO_IN()
//{
//	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
//}

void SPI_SoftWare_AF_Init()
{
    //1.IO时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //2.MOSI CS SCL配置为推挽输出 MISO配置为上拉输入（默认读取到高电平）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 |  GPIO_Pin_3 |GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);


	//初始W25Q片选线关闭
    CS_OUT = 1;
    //SPI模式0  第一个边沿采样（提前转入数据）
    SCL_OUT = 0;

}


//起始信号 片选线打开
void SPI_Start()
{
    CS_OUT = 0;
}


//停止信号   片选线关闭
void SPI_Stop()
{
    CS_OUT = 1;
}


//交换一个字节(掩码方式) 返回值为从机发过来的数据
uint8_t SPI_SendByte(uint8_t Data)
{

    uint8_t recv = 0x00;
    //模式0
    for(int i = 0; i < 8; i++)
    {
        //SS下降沿 主机移出数据到MOSI线上 从机会把最高位数据移到MISO线
        MOSI_OUT = (( (Data & (0x80 >> i))) == 0 ? 0 : 1) ;
		
        //SCLK 第一个上升沿 主机和从机移入数据
        // 主机接收一下
        SCL_OUT  = 1;
        //recv本来就是0 所以如果接收到0 不用管 直接偏移就行了
        if(MISO_IN == 1) {
            recv |= (0x80 >> i);
        }
        SCL_OUT = 0;
    }

    return recv;
}


//交换一个字节(移位寄存器方式) 返回值为从机发过来的数据
uint8_t SPI_SendByte_2(uint8_t Data)
{

    //模式0
    for(int i = 0; i < 8; i++)
    {
        //SS下降沿 主机移出数据到MOSI线上 从机会把最高位数据移到MISO线
        MOSI_OUT = ((Data & 0x80) == 0 ? 0 : 1);
        Data <<= 1; //Data的最低位空出补0
        //SCLK 第一个上升沿 主机和从机移入数据
        // 主机接收一下
        SCL_OUT = 1;
        //recv本来就是0 所以如果接收到0 不用管 直接偏移就行了
        if(MISO_IN == 1) { Data |= 0x01 ;}
        SCL_OUT = 0;
    }
    return Data;
}






