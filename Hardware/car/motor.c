#include "motor.h"

void Motor_GPIO_Config(uint32_t Motor_GPIO_CLK, GPIO_TypeDef* Motor_GPIO_PORT, uint16_t Motor_GPIO_PIN)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(Motor_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = Motor_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(Motor_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_ResetBits(Motor_GPIO_PORT,Motor_GPIO_PIN);
}

void Set_Motorx_Rotate_Dir(Motor_t *motorx, Motor_Rotate_Dir_Enum_t motor_Dir)
{
	switch(motor_Dir)
	{
		case Motor_Rotate_Dir_Forward:  // 正转
			*(motorx->motor_A) = 1;
			*(motorx->motor_B) = 0;
			break;
		case Motor_Rotate_Dir_Reverse:  // 反转
			*(motorx->motor_A) = 0;
			*(motorx->motor_B) = 1;
			break;
	}
}
