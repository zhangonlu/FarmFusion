#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"

//�������� ��ȡ���͵�ƽ ������ʱΪ�ߵ�ƽ���������裩

#define KEY1 PEIn(4)
#define KEY2 PEIn(5)
#define KEY3 PEIn(6)
#define KEY4 PCIn(13)

#define KEY1_Press 1
#define KEY2_Press 2
#define KEY3_Press 3
#define KEY4_Press 4

void KEY_Init(void);
u8 KEY_Scan(void);

#endif
