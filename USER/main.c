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
// Flash�� ��0x0800 0000 ��ͷ�ĵ�ַ  const���ε�Ϊ���� �洢��FLASh(���ô洢 �൱��Ӳ��)
// RAM��(�ڴ���)��0x2000 0000 ��ͷ�ĵ�ַ
// �����ַ��0x40000000 ��ͷ�ĵ�ַ


TaskHandle_t StartTask_Handler;//������
void TaskHandle_START(void *pvParameters);//��ʼ����������
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
        while (1); // ִֹͣ��
    }
    //AT24C02��ȡ��ֵ
    u8 data[3] = {0};
    Read_Data(data,0x0,3);
    Hum_yuzhi = data[0];
    Tem_yuzhi = data[1];
    Light_yuzhi = data[2];
    //������ʼ����
    xTaskCreate( (TaskFunction_t )TaskHandle_START,      //��ʼ������
                 (const char*    )"start",               //��������
                 (uint16_t       )512,        			 //�����ջ��С
                 (void*          )NULL,                  //���ݸ��������Ĳ���
                 (UBaseType_t    )1,      				 //�������ȼ�
                 (TaskHandle_t*  )&StartTask_Handler );  //������

    vTaskStartScheduler();  //�����������



}

void TaskHandle_OLED (void *pvParameters);//OLED�˵�������
void TaskHandle_KEY_Scan(void *pvParameters);//����ɨ��������
void TaskHandle_SensorData(void *pvParameters);//���������ݻ�ȡ����
void TaskHandle_WIFI(void *pvParameters);
void TaskHandle_START(void *pvParameters)//��ʼ����������
{
    taskENTER_CRITICAL();
    //�����ź���

    xTaskCreate(TaskHandle_OLED ,  "OLED��ʾ����",  128,  NULL, 4,  NULL);
    xTaskCreate(TaskHandle_SensorData ,  "���������ݻ�ȡ����",  128,  NULL, 3,  NULL);
    xTaskCreate(TaskHandle_KEY_Scan ,  "����ɨ������",  128,  NULL, 5,  NULL);
    xTaskCreate(TaskHandle_WIFI ,  "WIFIͨ������",  512,  NULL, 3,  NULL);

    vTaskDelete(StartTask_Handler);  //ɾ����ʼ���񣬵�����������ָ��

    /*------- �ٽ������� -------------*/
    taskEXIT_CRITICAL();
}

int flag = 0;
extern  Menu_table table[17];

void TaskHandle_OLED (void *pvParameters)//OLED�˵�������
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



//����֡��ʽ: ͷ��0x5555��+����+β(0xFFFF)
//���ݣ���һ���ֽڵ�8bitΪLED1 LED2 LED3 BEEP

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
            //���յ�OK
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
		      //�����ݷ��͸���λ��
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