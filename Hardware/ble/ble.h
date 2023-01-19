#ifndef __BLE_H
#define __BLE_H

#include "sys.h"

#include "usart.h"

/* BLE commond --------------------------------------------*/

typedef enum{
	BLE_Car_NoCommond = 1,				// 无命令
	BLE_Car_Forward,					// 前进
	BLE_Car_Backward,					// 后退
	BLE_Car_TurnLeft,					// 左转
	BLE_Car_TurnRight,					// 右转
	BLE_Car_Move,						// 移动
	BLE_Car_Stop,						// 停止
	BLE_Car_Up_AllDuty,					// 提高所有车轮的占空比
	BLE_Car_Low_AllDuty,				// 降低所有车轮的占空比
	BLE_Car_Up_AllDuty_ChangeStep,		// 提高所有车轮的占空比变化步长
	BLE_Car_Low_AllDuty_ChangeStep,		// 降低所有车轮的占空比变化步长
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
