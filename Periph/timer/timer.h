#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

void Timer1_Pwm_Init(uint16_t arr, uint16_t psc);
void Timer2_Pwm_Init(uint16_t arr, uint16_t psc);
void Timer3_Pwm_Init(uint16_t arr, uint16_t psc);
void Timer4_Pwm_Init(uint16_t arr, uint16_t psc);

void Set_TIMxCH4_Duty(TIM_TypeDef* TIMx, uint8_t duty);
uint8_t Get_TIMxCH4_Duty(TIM_TypeDef* TIMx);
void Set_TIM1CH4_Duty(uint8_t duty);
void Set_TIM2CH4_Duty(uint8_t duty);
void Set_TIM3CH4_Duty(uint8_t duty);
void Set_TIM4CH4_Duty(uint8_t duty);
uint8_t Get_TIM1CH4_Duty(void);
uint8_t Get_TIM2CH4_Duty(void);
uint8_t Get_TIM3CH4_Duty(void);
uint8_t Get_TIM4CH4_Duty(void);

#endif /* __TIMER_H */
