#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "misc.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "led.h"
#include "car.h"

/*================================================
	任务
================================================*/

// 开始任务
#define START_TASK_PRIO		1
#define START_STK_SIZE 		128
TaskHandle_t StartTask_Handler;
void Start_Task(void *pvParameters);

// LED任务
#define LED_FLASH_TASK_PRIO		1
#define LED_FLASH_STK_SIZE 		128
TaskHandle_t LEDFlashTask_Handler;
void Led_Flash_Task(void *pvParameters);

// 小车任务
#define CAR_TASK_PRIO		1
#define CAR_STK_SIZE 		1000
TaskHandle_t CarTask_Handler;
void Car_Task(void *pvParameters);

#endif /* __MAIN_H */
