#include "at24c02.h"
#include "delay.h"

//дʱ�޷��Զ���ҳ 
//һҳ16�ֽ� ����16�ֽڽ�����д��ҳ�� ����֮ǰ������ 
//����д��ʱ Ҫ���ʵ���Զ���ҳ 
/*
���� �� addr :0x012 д�� 35�ֽ�
	��ҳʣ���ֽ����� 8 - ��addr % 8��;	
*/
void at24c02_write(u8 addr,u8 *data,u8 len)
{
	u8 offeset = addr - (addr % 8);
	//��ǰҳ����һ��ҳ
	if(offeset != 0)
	{
	   IIC_SendData(AT24C02,addr,data,offeset);
		addr += offeset;
		len-=offeset;
		data+=offeset;
		delay_ms(10);
	}
	
	//�鿴���ж���ҳ
	offeset = 8;
	while(len / offeset > 0)
	{
		IIC_SendData(AT24C02,addr,data,offeset);
		addr += offeset;
		len-=offeset;
		data+=offeset;
		delay_ms(10);
	}
	
	//����ʣ���ֽ�
	IIC_SendData(AT24C02,addr,data,len);
	
}

void at24c02_read(u8 addr,u8 *data,u8 len)
{

  IIC_RecvData(AT24C02,addr,data,len);

}
