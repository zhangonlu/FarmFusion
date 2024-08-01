#include "oledmenu.h"
#include "oled.h"
#include "key.h"
void (*current_operation_index)();       //ִ�е�ǰ��������
uint8_t func_index = 0;                  //��ǰ����ִ��λ��


Menu_table table[17] =
{
    {0, 0, 1, (*fun0)},     //һ������

    {1, 2, 5, (*fun1)},     //������ģ��
    {2, 3, 12, (*fun2)},     //ͨ��ģ��
    {3, 4, 13, (*fun3)},     //��ֵ����
    {4, 1, 0, (*fun4)},     //�����˵� Back

    {5, 6, 9, (*fun5)},      //�����˵�1  ��ʪ��
    {6, 7, 10, (*fun6)},      //�����˵�  LED
    {7, 8, 11, (*fun7)},      //�����˵�  ����
    {8, 5, 1, (*fun8)},      //�����˵�   ����

    {9, 9, 5, (*fun9)},      //�����˵�  ��ʪ������
    {10, 10, 10, (*fun10)},      //LED 
    {11, 11, 11, (*fun11)},      //����  
	
    {12, 12, 12, (*fun12)},      //ESP8266ͨ��  
    {13, 13, 13, (*fun13)},      //��ֵ����  
	
    {14, 15, 0, (*fun14)},      //  LED1��
	
    {15, 16, 0, (*fun15)},      //�����˵�  3-2
    {16, 13, 0, (*fun16)},      //�����˵�  3-3
};


const char *model1[] = {
	"������ģ��",
	"ͨ��ģ��",
	"��ֵ����",
	"�˳�",

}; 

const char *model2[] = {
	"��ʪ��",
	"�ƹ�",
	"����",
	"�˳�"
};
//num ����������
void OLED_Running_Page1(int num)
{
	
		int flag = 1;
	    OLED_Clear();

	
		for(int i = 0; i < 4;i++)
		{
			if( (num % 4) == i)
				flag = 0;
			    if( num < 4)
					 OLED_ShowChineseString(30,i*16,(u8 *)model1[i],12,flag);
				else if(num < 8)
					 OLED_ShowChineseString(30,i*16,(u8 *)model2[i],12,flag);
			flag = 1;
		}
	
	
	  OLED_Refresh();
	
	
}





void fun0()
{
    int i,j ;
    int page_state = func_index;
    for(i = 0; i < 128; i+=1)
    {
        OLED_Clear();
        OLED_ShowPicture(i,0,128,64,(u8 *)BMP1,0);
        OLED_Refresh();
        //�������л�ʱ ��ϵ�ǰ���� ���н����л�
        if(page_state != func_index) break;
    }

}

void fun1()
{
	 OLED_Running_Page1(0);
}

void fun2()
{
	 OLED_Running_Page1(1);
}
void fun3() {
	 OLED_Running_Page1(2);
}

void fun4()
{
	 OLED_Running_Page1(3);
}
void fun5() {
	OLED_Running_Page1(4);

}

void fun6() {
	OLED_Running_Page1(5);

}
void fun7()   {
	OLED_Running_Page1(6);

}
void fun8() {
	OLED_Running_Page1(7);
}
#include "dht11.h"
//extern DHT11_t dht11_data;
extern u16 light;
extern double vol;
void fun9() {

    OLED_Clear();
    OLED_ShowChineseString(0,20,"�¶�",12,1);
    char buf[10] = "";
    sprintf(buf,":%.2lf\r\n",dht11_data.Tem);
    OLED_ShowString(25,20,(u8 *)buf,12,1);
    OLED_ShowChineseString(0,42,"ʪ��",12,1);
    sprintf(buf,":%.2lf\r\n",dht11_data.Hum);
    OLED_ShowString(25,42,(u8 *)buf,12,1);
	
	OLED_ShowChineseString(70,20,"����",12,1);
    sprintf(buf,":%d\r\n",light);
    OLED_ShowString(100,20,(u8 *)buf,12,1);
    OLED_ShowChineseString(70,42,"��ѹ",12,1);
    sprintf(buf,":%.1lf\r\n",vol);
    OLED_ShowString(100,42,(u8 *)buf,12,1);
	
    OLED_Refresh();

}

#include "led.h"
#include "pwm.h"
//�ƶ���λ��
u8 count = 0;
//ȷ�ϼ�λ��
u8 LED_sure_flag = 0;
const char *str[] = {
	"LED1",
	"LED2",
	"LED3",
	"BACK"
};
void fun10() {
	
	OLED_Clear();
	
	//ȷ�ϼ������� ����ǰcountѡ�е�λ�÷�ת
    if(LED_sure_flag == 1)
	{
		switch(count)
		{
			case 0 : LED1 = !LED1; break;
			case 1 : flg = !flg;   break;
			case 2 : LED3 =  !LED3; break;
			case 3 : func_index = 6;  count = 0; break;	
		}
		LED_sure_flag = 0;	
	}		
	
	int flag = 1;
	for(int i = 0;i < 4;i++)
	{
		if(count == i)
		{
			flag = 0;
			 if(count != 3)
			 OLED_DrawCircle(12,16*i+8,8);
			 else 
				OLED_DrawCircle(12,16*i+7,8); 
		}	
		OLED_ShowString(30,i*16,(u8 *)str[i],16,flag);
		flag = 1;
	}
	
    OLED_Refresh();
	
	
	
	

}

//
const char *str_fs[] = {
	"CLOSE",
	"POWER ONE",
	"POWER TWO",
	"POWER THREE",
	"BACK"
};
u8 FS_count = 0;
u8 FS_sure_flag = 0;
void fun11() {
	
		OLED_Clear();
	
	//ȷ�ϼ������� ����ǰcountѡ�е�λ�÷�ת
    if(FS_sure_flag == 1)
	{
		switch(FS_count)
		{
			case 0 : fs_power = 0; break;
			case 1 : fs_power = 1;   break;
			case 2 : fs_power = 2; break;
			case 3 : fs_power = 3;   break;
			case 4 : func_index = 7; FS_count = 0;  break;		
		}
		FS_sure_flag = 0;	
	}		
	
	int flag = 1;
	for(int i = FS_count;i <  4 + FS_count;i++)
	{
		if( (FS_count % 4) == (i%4) )
		{
			flag = 0;
			 if( (FS_count % 4) != 3)
			    OLED_DrawCircle(12,16*(i%4)+8,8);
			 else 
				OLED_DrawCircle(12,16*(i%4)+7,8); 
		}	
		OLED_ShowString(30,i*16,(u8 *)str_fs[i%5],16,flag);
		flag = 1;
	}
	
    OLED_Refresh();
	
}


#include "esp8266.h"
//esp8266
const char *esp_model[] = {
	"�ر�",
	"�ȵ�ģʽ",
	"����ģʽ",
	"�˳�"
};
u8 esp_status = 0;
u8 esp_count = 0;
u8 wifi_status = 0;
void fun12() {
	
	OLED_Clear();
    if(esp_status == 1)
	{
		switch(esp_count)
		{
			case 0 : ESP8266_Close(); wifi_status = 0;       break;
			case 1 : ESP8266_MODEL2_Init();  wifi_status = 1;  break;
			case 2 : ESP8266_MODEL2_Init();  wifi_status = 2; break;
			case 3 : func_index = 1;  esp_count = 0; break;	
		}
		esp_status = 0;	
	}		
	
	int flag = 1;
	for(int i = 0;i < 4;i++)
	{
		if(esp_count == i)
		{
			flag = 0;
			 if(esp_count != 3)
			 OLED_DrawCircle(12,16*i+8,8);
			 else 
				OLED_DrawCircle(12,16*i+7,8); 
		}	
		OLED_ShowChineseString(30,i*16,(u8 *)esp_model[i],16,flag);
		flag = 1;
	}
    	
    OLED_Refresh();
}

//��ֵ����
#include "w25q.h"
const char *yuzhi[] = {
    "�¶���ֵ",
	"ʪ����ֵ",
	"������ֵ",
	"�˳�"
};

u8 yuzhi_count = 0;
u8 yuzhi_state = 0;
 extern u8 Hum_yuzhi;  
 extern u8 Tem_yuzhi;  
 extern u8 Light_yuzhi;
void fun13() 
{
   	OLED_Clear();
    if(yuzhi_state == 1)
	{
		switch(yuzhi_count)
		{
			case 0 : Hum_yuzhi+=2;   break;
			case 1 : Tem_yuzhi+=2;   break;
			case 2 : Light_yuzhi+=2; break;
			case 3 : func_index = 1;  yuzhi_count = 0; break;	
		}
		u8 data[3] = {Hum_yuzhi,Tem_yuzhi,Light_yuzhi};
		Earse_Section(0x00000000);
		Program_Page(0x00000000,data,3); 
		yuzhi_state = 0;	
	}		
	
	int flag = 1;
	for(int i = 0;i < 4;i++)
	{
		if(yuzhi_count == i)
		{
			flag = 0;
			 if(yuzhi_count != 3)
			 OLED_DrawCircle(12,16*i+8,8);
			 else 
				OLED_DrawCircle(12,16*i+7,8); 
		}	
		OLED_ShowChineseString(30,i*16,(u8 *)yuzhi[i],16,flag);
		switch(i)
		{
			case 0:  OLED_ShowNum(96,i*16,Hum_yuzhi,2,16,flag); break;
			case 1:   OLED_ShowNum(96,i*16,Tem_yuzhi,2,16,flag);break;
			case 2:   OLED_ShowNum(96,i*16,Light_yuzhi,2,16,flag);break;
		}
		
		flag = 1;
	}
    	
    OLED_Refresh(); 	 		
}


void fun14()  {
	OLED_Clear();
    char buf[10] = "";
	LED1 = !LED1;
    sprintf(buf,"LED1: %s\r\n",LED1 == 0?"open" : "close");
    OLED_ShowString(30,0,(u8 *)buf,16,1);
	
    sprintf(buf,"LED2: %s\r\n",LED2 == 0?"open" : "close");
    OLED_ShowString(30,20,(u8 *)buf,16,1);
	sprintf(buf,"LED3: %s\r\n",LED3 == 0?"open" : "close");
	OLED_ShowString(30,40,(u8 *)buf,16,1);
	OLED_DrawCircle(20,9,8);
    OLED_Refresh();	

}
void fun15() {

}
void fun16() {

}


