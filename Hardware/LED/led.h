#ifndef __LED_H
#define __LED_H

#include "sys.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define LED PCout(13)

typedef enum{
	LED_ON = 0,
	LED_OFF = 1
}LED_State_Typedef;

void LED_GPIO_Config(uint32_t LED_GPIO_CLK, GPIO_TypeDef* LED_GPIO_PORT, uint16_t LED_GPIO_PIN);
void LED_Init(void);

#endif /* __LED_H */
