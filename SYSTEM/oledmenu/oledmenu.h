#ifndef __OLEDMENU_H_
#define __OLEDMENU_H_

#include "sys.h"


typedef struct
{
    uint8_t current;  //当前状态索引号
    uint8_t next;     //向下按 
    uint8_t enter;    //确定键按
    void (*current_operation)(void);//当前状态应该执行的操作
}Menu_table;
extern u8 LED_sure_flag;
extern u8 count; 

extern u8 FS_sure_flag;
extern u8 FS_count;

extern u8 esp_count;
extern u8 esp_status;
extern u8 esp_model_select;

extern u8 yuzhi_count;
extern u8 yuzhi_state;
void fun0(void);
void fun1(void);
void fun2(void);
void fun3(void);
void fun4(void);
void fun5(void);
void fun6(void);
void fun7(void);
void fun8(void);
void fun9(void);
void fun10(void);
void fun11(void);
void fun12(void);
void fun13(void);
void fun14(void);
void fun15(void);
void fun16(void);

extern Menu_table table[17];
extern void (*current_operation_index)();       //执行当前操作函数
extern uint8_t func_index ;                  //当前索引执行位置
extern int key_state ;                           //按键状态 
extern u8 wifi_status ;

void KEY_Handler(void);
#endif
