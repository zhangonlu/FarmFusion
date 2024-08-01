#include "at24c02.h"
#include "delay.h"

//写时无法自动增页 
//一页16字节 超过16字节将重新写在页首 覆盖之前的数据 
//所以写入时 要软件实现自动增页 
/*
例如 在 addr :0x012 写入 35字节
	当页剩余字节数： 8 - （addr % 8）;	
*/
void at24c02_write(u8 addr,u8 *data,u8 len)
{
	u8 offeset = addr - (addr % 8);
	//当前页不是一整页
	if(offeset != 0)
	{
	   IIC_SendData(AT24C02,addr,data,offeset);
		addr += offeset;
		len-=offeset;
		data+=offeset;
		delay_ms(10);
	}
	
	//查看还有多少页
	offeset = 8;
	while(len / offeset > 0)
	{
		IIC_SendData(AT24C02,addr,data,offeset);
		addr += offeset;
		len-=offeset;
		data+=offeset;
		delay_ms(10);
	}
	
	//发送剩余字节
	IIC_SendData(AT24C02,addr,data,len);
	
}

void at24c02_read(u8 addr,u8 *data,u8 len)
{

  IIC_RecvData(AT24C02,addr,data,len);

}
