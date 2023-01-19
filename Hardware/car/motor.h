#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

typedef enum{
	Motor_Rotate_Dir_Forward,		// 电机正转
	Motor_Rotate_Dir_Reverse		// 电机反转
}Motor_Rotate_Dir_Enum_t;

typedef struct Motor_t{
	vu32 *motor_A;		// 电机引脚A
	vu32 *motor_B;		// 电机引脚B
	sc8 *en_Pin;		// 使能引脚
	uint8_t duty;		// 电机占空比
	
	void (*Set_Dir)(struct Motor_t*, Motor_Rotate_Dir_Enum_t);  // 设置电机正反转
	void (*Update_Duty)(struct Motor_t*);  // 更新占空比
}Motor_t;

void Motor_GPIO_Config(uint32_t Motor_GPIO_CLK, GPIO_TypeDef* Motor_GPIO_PORT, uint16_t Motor_GPIO_PIN);
void Set_Motorx_Rotate_Dir(Motor_t *motorx, Motor_Rotate_Dir_Enum_t motor_Dir);

#endif /* __MOTOR_H */
