#include "spi.h"


//���ģ��SPI
//����MOSI-->PB5
//MISO ---> PB4
//SCL ----> PB3
//W25Q CS ---> PB2

//u8 MISO_IN()
//{
//	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
//}

void SPI_SoftWare_AF_Init()
{
    //1.IOʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //2.MOSI CS SCL����Ϊ������� MISO����Ϊ�������루Ĭ�϶�ȡ���ߵ�ƽ��
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


	//��ʼW25QƬѡ�߹ر�
    CS_OUT = 1;
    //SPIģʽ0  ��һ�����ز�������ǰת�����ݣ�
    SCL_OUT = 0;

}


//��ʼ�ź� Ƭѡ�ߴ�
void SPI_Start()
{
    CS_OUT = 0;
}


//ֹͣ�ź�   Ƭѡ�߹ر�
void SPI_Stop()
{
    CS_OUT = 1;
}


//����һ���ֽ�(���뷽ʽ) ����ֵΪ�ӻ�������������
uint8_t SPI_SendByte(uint8_t Data)
{

    uint8_t recv = 0x00;
    //ģʽ0
    for(int i = 0; i < 8; i++)
    {
        //SS�½��� �����Ƴ����ݵ�MOSI���� �ӻ�������λ�����Ƶ�MISO��
        MOSI_OUT = (( (Data & (0x80 >> i))) == 0 ? 0 : 1) ;
		
        //SCLK ��һ�������� �����ʹӻ���������
        // ��������һ��
        SCL_OUT  = 1;
        //recv��������0 ����������յ�0 ���ù� ֱ��ƫ�ƾ�����
        if(MISO_IN == 1) {
            recv |= (0x80 >> i);
        }
        SCL_OUT = 0;
    }

    return recv;
}


//����һ���ֽ�(��λ�Ĵ�����ʽ) ����ֵΪ�ӻ�������������
uint8_t SPI_SendByte_2(uint8_t Data)
{

    //ģʽ0
    for(int i = 0; i < 8; i++)
    {
        //SS�½��� �����Ƴ����ݵ�MOSI���� �ӻ�������λ�����Ƶ�MISO��
        MOSI_OUT = ((Data & 0x80) == 0 ? 0 : 1);
        Data <<= 1; //Data�����λ�ճ���0
        //SCLK ��һ�������� �����ʹӻ���������
        // ��������һ��
        SCL_OUT = 1;
        //recv��������0 ����������յ�0 ���ù� ֱ��ƫ�ƾ�����
        if(MISO_IN == 1) { Data |= 0x01 ;}
        SCL_OUT = 0;
    }
    return Data;
}






