#ifndef __PWM_H_
#define __PWM_H_

#include "sys.h"

//PWM驱动呼吸灯
void PWM_Init(void);
//PWM驱动风扇
void PWM_Fs_Init(void);
void LED_Toggri(void);
//风扇开关
void FS_Controller(void);

//呼吸灯状态
extern u8 flg;

//风扇档位
extern u8 fs_power;


#endif
