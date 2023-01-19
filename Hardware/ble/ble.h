#ifndef __BLE_H
#define __BLE_H

#include "sys.h"

#include "usart.h"

/* BLE commond --------------------------------------------*/

typedef enum{
	BLE_Car_NoCommond = 1,				// ������
	BLE_Car_Forward,					// ǰ��
	BLE_Car_Backward,					// ����
	BLE_Car_TurnLeft,					// ��ת
	BLE_Car_TurnRight,					// ��ת
	BLE_Car_Move,						// �ƶ�
	BLE_Car_Stop,						// ֹͣ
	BLE_Car_Up_AllDuty,					// ������г��ֵ�ռ�ձ�
	BLE_Car_Low_AllDuty,				// �������г��ֵ�ռ�ձ�
	BLE_Car_Up_AllDuty_ChangeStep,		// ������г��ֵ�ռ�ձȱ仯����
	BLE_Car_Low_AllDuty_ChangeStep,		// �������г��ֵ�ռ�ձȱ仯����
}BLE_Car_Commond_Enum_t;

/* BLE function -------------------------------------------*/

void BLE_Init(void);

void BLE_Send_Num(uint16_t num);
void BLE_Send_Char(uint8_t chr);
void BLE_Send_String(uint8_t *str);
void BLE_Postback(void);
uint8_t BLE_Get_Char(uint16_t addr);
uint8_t *BLE_Get_String(void);

#endif /* __BLE_H */
