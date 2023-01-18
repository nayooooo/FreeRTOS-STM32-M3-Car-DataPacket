#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

typedef enum{
	Motor_Rotate_Dir_Forward,		// �����ת
	Motor_Rotate_Dir_Reverse		// �����ת
}Motor_Rotate_Dir_Enum_t;

typedef struct Motor_t{
	vu32 *motor_A;		// �������A
	vu32 *motor_B;		// �������B
	sc8 *en_Pin;		// ʹ������
	uint8_t duty;		// ���ռ�ձ�
	
	void (*Set_Dir)(struct Motor_t*, Motor_Rotate_Dir_Enum_t);  // ���õ������ת
	void (*Update_Duty)(struct Motor_t*);  // ����ռ�ձ�
}Motor_t;

void Motor_GPIO_Config(uint32_t Motor_GPIO_CLK, GPIO_TypeDef* Motor_GPIO_PORT, uint16_t Motor_GPIO_PIN);
void Set_Motorx_Rotate_Dir(Motor_t *motorx, Motor_Rotate_Dir_Enum_t motor_Dir);

#endif /* __MOTOR_H */
