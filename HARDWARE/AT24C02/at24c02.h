#ifndef __AT24C02_H_
#define __AT24C02_H_

#include "iic.h"

//…Ë±∏µÿ÷∑
#define AT24C02 0xA0


void at24c02_write(u8 addr,u8 *data,u8 len);

void at24c02_read(u8 addr,u8 *data,u8 len);


#endif
