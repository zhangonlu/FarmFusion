//#ifndef __SYS_H_
//#define __SYS_H_

//#include "stm32f4xx.h"

////����һ���Ĵ���ʵ�ʵ�ַ �õ�λ����ַ����Ч������ַ��
//#define BitBand(addr,n)   0x42000000 + ((addr & 0x000FFFFF) << 5) + (n<<2)
////��ֵ��ַ��
//#define Mem_Addr(addr)  *((volatile unsigned long *)(addr))

//#define Bit_Addr(addr,n) Mem_Addr(BitBand(addr,n))

////�ҵ���Ҫ�����ļĴ���ʵ�ʵ�ַ(STM32F4)
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

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAOut(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAIn(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 
	
#define PBOut(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBIn(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 
	
#define PCOut(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCIn(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 
	
#define PDOut(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDIn(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 
	
#define PEOut(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEIn(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����
	
#define PFOut(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFIn(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����
	
#define PGOut(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGIn(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
	
#define PHOut(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHIn(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����
	
#define PIOut(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIIn(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����

#endif












