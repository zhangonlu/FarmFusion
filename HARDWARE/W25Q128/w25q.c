#include "w25q.h"

 
void W25Q_Init()
{
    SPI_SoftWare_AF_Init();
}

//厂商ID和设备ID
void W25Q_Send(u8 *MID,u16 *DID)
{
    SPI_Start();
    SPI_SendByte(w25Q64_JEDEC_ID);
    *MID = SPI_SendByte(w25Q64_DUMMY_BYTE);
    *DID =  SPI_SendByte(w25Q64_DUMMY_BYTE);
    *DID <<= 8;
    *DID |= SPI_SendByte(w25Q64_DUMMY_BYTE);
    SPI_Stop();
}

//写使能
void Write_Enable()
{
 	SPI_Start();	
	SPI_SendByte(w25Q64__WRITE_ENABLE);
	SPI_Stop();
}

//读取一个数组数据
//先发送读指令 在分三次发送24位的地址（每次只能发送8位） 
void  Read_Data(u8 *data,u32 addr,u16 len)
{
    SPI_Start();	
	SPI_SendByte(w25Q64_READ_DATA);
	SPI_SendByte(addr >> 16);
	SPI_SendByte(addr >> 8);
	SPI_SendByte(addr);
	for(int i = 0;i < len;i++)
	{
		data[i] = SPI_SendByte(w25Q64_DUMMY_BYTE);
	}

	SPI_Stop();
}


//等待忙状态结束
//通过读取读状态寄存器的Busy位
void Wait_BusyStatus()
{
	 
	 SPI_Start();
	 //死等忙状态结束
	 while((SPI_SendByte(w25Q64_READ_STATUS_REGISTER_1) & 0x01) == 1);
	 SPI_Stop();
}

//页编程函数
void Program_Page(u32 addr,u8 *data,u16 len)
{
	Write_Enable();
   	SPI_Start();
	SPI_SendByte(w25Q64_PAGE_PROGRAM);
	SPI_SendByte(addr >> 16);
	SPI_SendByte(addr >> 8);
	SPI_SendByte(addr);
	for(int i = 0;i < len;i++)
    {
		SPI_SendByte(data[i]);
	}	
	SPI_Stop();
	//写完等待忙状态结束
	Wait_BusyStatus();
}	


//扇区擦除
void Earse_Section(u32 addr)
{
	Write_Enable();
	SPI_Start();
	SPI_SendByte(w25Q64_SECTOR_ERASE__4KB);
	SPI_SendByte(addr >> 16);
	SPI_SendByte(addr >> 8);
    SPI_SendByte(addr);	
	SPI_Stop();
	Wait_BusyStatus();
}




//硬件实现
//void W25Q_Init()
//{
//    SPI_Hard_Init();
//}

////厂商ID和设备ID
//void W25Q_Send(u8 *MID,u16 *DID)
//{
//    SPI_Hard_Start();
//    SPI_Hard_SwapData(w25Q64_JEDEC_ID);
//    *MID = SPI_Hard_SwapData(w25Q64_DUMMY_BYTE);
//    *DID =  SPI_Hard_SwapData(w25Q64_DUMMY_BYTE);
//    *DID <<= 8;
//    *DID |= SPI_Hard_SwapData(w25Q64_DUMMY_BYTE);
//    SPI_Hard_Stop();
//}

////写使能
//void Write_Enable()
//{
// 	SPI_Hard_Start();	
//	SPI_Hard_SwapData(w25Q64__WRITE_ENABLE);
//	SPI_Hard_Stop();
//}

////读取一个数组数据
////先发送读指令 在分三次发送24位的地址（每次只能发送8位） 
//void  Read_Data(u8 *data,u32 addr,u16 len)
//{
//    SPI_Hard_Start();	
//	SPI_Hard_SwapData(w25Q64_READ_DATA);
//	SPI_Hard_SwapData(addr >> 16);
//	SPI_Hard_SwapData(addr >> 8);
//	SPI_Hard_SwapData(addr);
//	for(int i = 0;i < len;i++)
//	{
//		data[i] = SPI_Hard_SwapData(w25Q64_DUMMY_BYTE);
//	}

//	SPI_Hard_Stop();
//}


////等待忙状态结束
////通过读取读状态寄存器的Busy位
//void Wait_BusyStatus()
//{
//	 
//	 SPI_Hard_Start();
//	 //死等忙状态结束
//	 while((SPI_Hard_SwapData(w25Q64_READ_STATUS_REGISTER_1) & 0x01) == 1);
//	 SPI_Hard_Stop();
//}

////页编程函数
//void Program_Page(u32 addr,u8 *data,u16 len)
//{
//	Write_Enable();
//   	SPI_Hard_Start();
//	SPI_Hard_SwapData(w25Q64_PAGE_PROGRAM);
//	SPI_Hard_SwapData(addr >> 16);
//	SPI_Hard_SwapData(addr >> 8);
//	SPI_Hard_SwapData(addr);
//	for(int i = 0;i < len;i++)
//    {
//		SPI_Hard_SwapData(data[i]);
//	}	
//	SPI_Hard_Stop();
//	//写完等待忙状态结束
//	Wait_BusyStatus();
//}	


////扇区擦除
//void Earse_Section(u32 addr)
//{
//	Write_Enable();
//	SPI_Hard_Start();
//	SPI_Hard_SwapData(w25Q64_SECTOR_ERASE__4KB);
//	SPI_Hard_SwapData(addr >> 16);
//	SPI_Hard_SwapData(addr >> 8);
//    SPI_Hard_SwapData(addr);	
//	SPI_Hard_Stop();
//	Wait_BusyStatus();
//}


