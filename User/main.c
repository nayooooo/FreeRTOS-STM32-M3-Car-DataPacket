/**
 * @file main.c
 * @brief С��
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
	
	// ������ʼ����
    xTaskCreate((TaskFunction_t )Start_Task,            // ������
                (const char*    )"Start_Task",          // ��������
                (uint16_t       )START_STK_SIZE,        // �����ջ��С
                (void*          )NULL,                  // ���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       // �������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   // ������
	vTaskStartScheduler();          // �����������
}

// ��ʼ����������
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();           // �����ٽ���
    // ����LED����
    xTaskCreate((TaskFunction_t )Led_Flash_Task,
                (const char*    )"Led_Flash_Task",
                (uint16_t       )LED_FLASH_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_FLASH_TASK_PRIO,
                (TaskHandle_t*  )&LEDFlashTask_Handler);
    // ����С������
    xTaskCreate((TaskFunction_t )Car_Task,
                (const char*    )"Car_Task",
                (uint16_t       )CAR_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CAR_TASK_PRIO,
                (TaskHandle_t*  )&CarTask_Handler);
    vTaskDelete(NULL);				// ɾ����ʼ��������
    taskEXIT_CRITICAL();			// �˳��ٽ���
}

// LED��˸����
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

// С������
void Car_Task(void *pvParameters)
{
	Car_Init();
	
	while(1)
	{
		if(USART3_RX_STA&USART3_RX_STA_REC_END) {  // ���յ���һ������
			;
		}
		vTaskResume(LEDFlashTask_Handler);
		vTaskDelay(1);
	}
}
