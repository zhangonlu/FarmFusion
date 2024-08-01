#include "light.h"

//AD转换之光敏电阻
//PA0 ---> ADC123_IN0

void ADC_Light_Init()
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   
   	//2.配置GPIOA pin0为模拟输入
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}



#include "adc.h"
u16 GET_Average_Light()
{
    u16 temp = 0;
    for(int i = 0; i < 10; i++)
    {
        temp += Get_ADC(ADC1,0);
    }

    return temp/10;
}



