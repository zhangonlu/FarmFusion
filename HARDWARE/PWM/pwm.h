#ifndef __PWM_H_
#define __PWM_H_

#include "sys.h"

//PWM����������
void PWM_Init(void);
//PWM��������
void PWM_Fs_Init(void);
void LED_Toggri(void);
//���ȿ���
void FS_Controller(void);

//������״̬
extern u8 flg;

//���ȵ�λ
extern u8 fs_power;


#endif
