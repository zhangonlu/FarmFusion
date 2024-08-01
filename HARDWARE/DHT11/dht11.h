#ifndef __DHT11_H_
#define __DHT11_H_
#include "sys.h"

//DHT11_DATA ----> PA3

#define DHT_DATA_OUT PAOut(3)
#define DHT_DATA_IN  PAIn(3)

typedef struct 
{
   u8  Hum_H;
   u8  Hum_L;
   u8  Tem_H;
   u8  Tem_L;
   double Hum;
   double Tem;	
}DHT11_t;

extern DHT11_t dht11_data;
u8 DHT_ReadByte(void);
u8 DHT_ReadData(DHT11_t *dht11);
void DHT11_Init(void);


#endif


