//#ifndef __SYS_H_
//#define __SYS_H_

//#include "stm32f4xx.h"

////输入一个寄存器实际地址 得到位带地址（等效别名地址）
//#define BitBand(addr,n)   0x42000000 + ((addr & 0x000FFFFF) << 5) + (n<<2)
////将值地址化
//#define Mem_Addr(addr)  *((volatile unsigned long *)(addr))

//#define Bit_Addr(addr,n) Mem_Addr(BitBand(addr,n))

////找到需要操作的寄存器实际地址(STM32F4)
//#define GPIOA_ODR_Addr  (0x40020000 + 0x14)
//#define GPIOB_ODR_Addr  (0x40020400 + 0x14)
//#define GPIOC_ODR_Addr  (0x40020800 + 0x14)
//#define GPIOD_ODR_Addr  (0x40020C00 + 0x14)
//#define GPIOE_ODR_Addr  (0x40021000 + 0x14)
//#define GPIOF_ODR_Addr  (0x40021400 + 0x14)
//#define GPIOG_ODR_Addr  (0x40021800 + 0x14)
//#define GPIOH_ODR_Addr  (0x40021C00 + 0x14)
//#define GPIOI_ODR_Addr  (0x40022000 + 0x14)


//#define GPIOA_IDR_Addr  (0x40020000 + 0x10)
//#define GPIOB_IDR_Addr  (0x40020400 + 0x10)
//#define GPIOC_IDR_Addr  (0x40020800 + 0x10)
//#define GPIOD_IDR_Addr  (0x40020C00 + 0x10)
//#define GPIOE_IDR_Addr  (0x40021000 + 0x10)
//#define GPIOF_IDR_Addr  (0x40021400 + 0x10)
//#define GPIOG_IDR_Addr  (0x40021800 + 0x10)
//#define GPIOH_IDR_Addr  (0x40021C00 + 0x10)
//#define GPIOI_IDR_Addr  (0x40022000 + 0x10)

//#define PAOut(n) Bit_Addr(GPIOA_ODR_Addr,n)
//#define PBOut(n) Bit_Addr(GPIOB_ODR_Addr,n)
//#define PCOut(n) Bit_Addr(GPIOC_ODR_Addr,n)
//#define PDOut(n) Bit_Addr(GPIOD_ODR_Addr,n)
//#define PEOut(n) Bit_Addr(GPIOE_ODR_Addr,n)
//#define PFOut(n) Bit_Addr(GPIOF_ODR_Addr,n)
//#define PGOut(n) Bit_Addr(GPIOG_ODR_Addr,n)
//#define PHOut(n) Bit_Addr(GPIOH_ODR_Addr,n)
//#define PIOut(n) Bit_Addr(GPIOI_ODR_Addr,n)

//#define PAIn(n) Bit_Addr(GPIOA_IDR_Addr,n)
//#define PBIn(n) Bit_Addr(GPIOB_IDR_Addr,n)
//#define PCIn(n) Bit_Addr(GPIOC_IDR_Addr,n)
//#define PDIn(n) Bit_Addr(GPIOD_IDR_Addr,n)
//#define PEIn(n) Bit_Addr(GPIOE_IDR_Addr,n)
//#define PFIn(n) Bit_Addr(GPIOF_IDR_Addr,n)
//#define PGIn(n) Bit_Addr(GPIOG_IDR_Addr,n)
//#define PHIn(n) Bit_Addr(GPIOH_IDR_Addr,n)
//#define PIIn(n) Bit_Addr(GPIOI_IDR_Addr,n)





//#endif

#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAOut(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAIn(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
	
#define PBOut(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBIn(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
	
#define PCOut(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCIn(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 
	
#define PDOut(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDIn(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 
	
#define PEOut(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEIn(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
	
#define PFOut(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFIn(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
	
#define PGOut(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGIn(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
	
#define PHOut(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHIn(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入
	
#define PIOut(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIIn(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

#endif












