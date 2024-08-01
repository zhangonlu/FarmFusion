#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "usart1.h"
#include "stdio.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "light.h"
#include "dma.h"
#include "adcdma.h"
#include "iic.h"
#include "at24c02.h"
#include "string.h"
#include "oled.h"
#include "w25q.h"
#include "dht11.h"
#include "esp8266.h"
#include "freertos.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "oledmenu.h"
#include "semphr.h"
// Flash区 ：0x0800 0000 开头的地址  const修饰的为常量 存储在FLASh(永久存储 相当于硬盘)
// RAM区(内存区)：0x2000 0000 开头的地址
// 外设地址：0x40000000 开头的地址


TaskHandle_t StartTask_Handler;//任务句柄
void TaskHandle_START(void *pvParameters);//开始任务任务函数
u8 Hum_yuzhi   = 0;
u8 Tem_yuzhi   = 0;
u8 Light_yuzhi = 0;
SemaphoreHandle_t xSemaphore;
int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    BEEP_Init();
    KEY_Init();
    EXTI_KEY_Init();
    USART1_Init();
    TIMER_Init();
    PWM_Init();
    ADCx_Init(ADC1);
    ADC_Light_Init();

    TIM7_Init();
    MyIIC_Init();
    OLED_Init();
    W25Q_Init();
    PWM_Fs_Init();
    PWM_Init();

    DHT11_Init();
    USART2_Init();


	 xSemaphore = xSemaphoreCreateBinary();
	    if (xSemaphore == NULL) {
        printf("Failed to create semaphore\n");
        while (1); // 停止执行
    }
    //AT24C02读取阈值
    u8 data[3] = {0};
    Read_Data(data,0x0,3);
    Hum_yuzhi = data[0];
    Tem_yuzhi = data[1];
    Light_yuzhi = data[2];
    //创建开始任务
    xTaskCreate( (TaskFunction_t )TaskHandle_START,      //开始任务函数
                 (const char*    )"start",               //任务名称
                 (uint16_t       )512,        			 //任务堆栈大小
                 (void*          )NULL,                  //传递给任务函数的参数
                 (UBaseType_t    )1,      				 //任务优先级
                 (TaskHandle_t*  )&StartTask_Handler );  //任务句柄

    vTaskStartScheduler();  //开启任务调度



}

void TaskHandle_OLED (void *pvParameters);//OLED菜单任务函数
void TaskHandle_KEY_Scan(void *pvParameters);//按键扫描任务函数
void TaskHandle_SensorData(void *pvParameters);//传感器数据获取函数
void TaskHandle_WIFI(void *pvParameters);
void TaskHandle_START(void *pvParameters)//开始任务任务函数
{
    taskENTER_CRITICAL();
    //创建信号量

    xTaskCreate(TaskHandle_OLED ,  "OLED显示任务",  128,  NULL, 4,  NULL);
    xTaskCreate(TaskHandle_SensorData ,  "传感器数据获取任务",  128,  NULL, 3,  NULL);
    xTaskCreate(TaskHandle_KEY_Scan ,  "按键扫描任务",  128,  NULL, 5,  NULL);
    xTaskCreate(TaskHandle_WIFI ,  "WIFI通信任务",  512,  NULL, 3,  NULL);

    vTaskDelete(StartTask_Handler);  //删除开始任务，调用了任务句柄指针

    /*------- 临界区结束 -------------*/
    taskEXIT_CRITICAL();
}

int flag = 0;
extern  Menu_table table[17];

void TaskHandle_OLED (void *pvParameters)//OLED菜单任务函数
{
    u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {
        current_operation_index = table[func_index].current_operation;
        (*current_operation_index)();
        vTaskDelay(10);
        // vTaskDelayUntil(&lastWakeTime,10);
    }


}

void TaskHandle_KEY_Scan(void *pvParameters)
{

    u32 lastWakeTime = xTaskGetTickCount();

    while(1)
    {
        vTaskDelay(5);
        switch(KEY_Scan())
        {
        case 0:
            break;
        case 1:
            func_index = 	table[func_index].next ;
            if(func_index == 10)
            {
                count++;
                if(count == 4)
                    count = 0;
            }
            if(func_index == 11)
            {
                FS_count++;
                if(FS_count == 5)
                    FS_count = 0;
            }
            if(func_index == 12)
            {
                esp_count++;
                if(esp_count == 4)
                    esp_count = 0;
            }
            if(func_index == 13)
            {
                yuzhi_count++;
                if(yuzhi_count == 4)
                    yuzhi_count = 0;
            }
            break;
        case 2:

            if(func_index == 10)
            {
                LED_sure_flag = 1;
            }
            if(func_index == 11)
            {
                FS_sure_flag = 1;
            }
            if(func_index == 12)
            {
                esp_status = 1;
            }
            if(func_index == 13)
            {
                yuzhi_state = 1;
            }
            func_index = table[func_index].enter;
            break;
        }
        //vTaskDelayUntil(&lastWakeTime,10);
    }
}

extern DHT11_t dht11_data;
extern LED_State LED_Status;
extern u16 DMA_Data[2];
void TaskHandle_SensorData(void *pvParameters)
{

    ADC_DMA_Init();
    u32 lastWakeTime = xTaskGetTickCount();

    while(1)
    {
        vTaskDelayUntil(&lastWakeTime,200);
        taskENTER_CRITICAL();
        DHT_ReadData(&dht11_data);
        taskEXIT_CRITICAL();

        LED_Toggri();
        FS_Controller();
        DMA_Value();

    }

}



//数据帧格式: 头（0x5555）+数据+尾(0xFFFF)
//数据：第一个字节的8bit为LED1 LED2 LED3 BEEP

void TaskHandle_WIFI(void *pvParameters)
{

    u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {
        vTaskDelayUntil(&lastWakeTime,600);
//	 if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {	
		if(ringbuffer.flag == 1)
		{
			char data[RingBuffer_Length+1];
            u8 len = RingBuffer_GetLength(&ringbuffer);
            RingBuffer_Read_Data(&ringbuffer, (u8 *)data,len );
            //接收到OK
            if( strstr(data,"LED1_open") != NULL)
            {
                LED1 = 1;
            }
			else if( strstr(data,"LED1_close") != NULL)
            {
                LED1 = 0;
            }
			
			ringbuffer.flag = 0;
		}
//	}	
		      //将数据发送给上位机
        u8 data[15];
        data[0] = 0x55;
        data[1] = BEEP;
        data[2] = fs_power;
        data[3] = LED1;
        data[4] = flg;
        data[5] = LED3;
        data[6] = dht11_data.Hum_H;
        data[7] = dht11_data.Tem_H;
        data[8] = light;
        data[9] =  Hum_yuzhi;
        data[10] = Tem_yuzhi;
        data[11] = Light_yuzhi;
        data[12] = 0xFF;
        data[13] = '\r';
        data[14] = '\n';
        TCP_SendData((u8 *)data,0,sizeof(data));
//	  xSemaphoreGive(xSemaphore);
    }
		
  

}
