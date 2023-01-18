/**
 * @file main.c
 * @brief 小车
 *
 * @encoding GB2312
 * @author yewan
 */

#include "main.h"

#include "stm32f10x_gpio.h"

void System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_init();
	USART1_Init(115200);
}

int main()
{
	System_Init();
	
	// 创建开始任务
    xTaskCreate((TaskFunction_t )Start_Task,            // 任务函数
                (const char*    )"Start_Task",          // 任务名称
                (uint16_t       )START_STK_SIZE,        // 任务堆栈大小
                (void*          )NULL,                  // 传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       // 任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   // 任务句柄
	vTaskStartScheduler();          // 开启任务调度
}

// 开始任务任务函数
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();           // 进入临界区
    // 创建LED任务
    xTaskCreate((TaskFunction_t )Led_Flash_Task,
                (const char*    )"Led_Flash_Task",
                (uint16_t       )LED_FLASH_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_FLASH_TASK_PRIO,
                (TaskHandle_t*  )&LEDFlashTask_Handler);
    // 创建小车任务
    xTaskCreate((TaskFunction_t )Car_Task,
                (const char*    )"Car_Task",
                (uint16_t       )CAR_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAR_TASK_PRIO,
                (TaskHandle_t*  )&CarTask_Handler);
    vTaskDelete(NULL);				// 删除开始任务（自身）
    taskEXIT_CRITICAL();			// 退出临界区
}

// LED闪烁任务
void Led_Flash_Task(void *pvParameters)
{
	LED_Init();
	
	while (1)
	{
		LED = LED_OFF;
		vTaskDelay(500);
		LED = LED_ON;
		vTaskDelay(500);
		vTaskSuspend(NULL);
	}
}

// 小车任务
void Car_Task(void *pvParameters)
{
	Car_Init();
	
	while(1)
	{
		if(USART3_RX_STA&USART3_RX_STA_REC_END) {  // 接收到了一包数据
			;
		}
		vTaskResume(LEDFlashTask_Handler);
		vTaskDelay(1);
	}
}
