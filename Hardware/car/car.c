#include "car.h"

#include <stdio.h>

/*=========================================================
	extern car action
=========================================================*/

static void Car_Wheels_Turn_Forward(void);
static void Car_Wheels_Turn_Backward(void);
static void Car_Wheels_TurnLeft(void);
static void Car_Wheels_TurnRight(void);
static void Car_Move(void);
static void Car_Stop(void);
static void Car_Up_Update_AllWheel_Duty(void);
static void Car_Low_Update_AllWheel_Duty(void);
static void Car_Up_AllDuty_ChangeStep(void);
static void Car_Low_AllDuty_ChangeStep(void);

/*=========================================================
	car wheels
=========================================================*/

static void Update_Car_Motor11_Duty(Motor_t* motorx)
{
	Set_TIM1CH4_Duty(motorx->duty);
}

static void Update_Car_Motor12_Duty(Motor_t* motorx)
{
	Set_TIM2CH4_Duty(motorx->duty);
}

static void Update_Car_Motor21_Duty(Motor_t* motorx)
{
	Set_TIM3CH4_Duty(motorx->duty);
}

static void Update_Car_Motor22_Duty(Motor_t* motorx)
{
	Set_TIM4CH4_Duty(motorx->duty);
}

Motor_t wheels[4] = {
	{  // wheel11
		.motor_A = &(PCout(14)),
		.motor_B = &(PCout(15)),
		.en_Pin = (sc8*)"PA11",
		.duty = 0,
		.Set_Dir = Set_Motorx_Rotate_Dir,
		.Update_Duty = Update_Car_Motor11_Duty
	},
	{  // wheel12
		.motor_A = &(PAout(1)),
		.motor_B = &(PAout(2)),
		.en_Pin = (sc8*)"PA3",
		.duty = 0,
		.Set_Dir = Set_Motorx_Rotate_Dir,
		.Update_Duty = Update_Car_Motor12_Duty
	},
	{  // wheel21
		.motor_A = &(PAout(5)),
		.motor_B = &(PAout(4)),
		.en_Pin = (sc8*)"PB1",
		.duty = 0,
		.Set_Dir = Set_Motorx_Rotate_Dir,
		.Update_Duty = Update_Car_Motor21_Duty
	},
	{  // wheel22
		.motor_A = &(PAout(7)),
		.motor_B = &(PAout(6)),
		.en_Pin = (sc8*)"PB9",
		.duty = 0,
		.Set_Dir = Set_Motorx_Rotate_Dir,
		.Update_Duty = Update_Car_Motor22_Duty
	},
};

/*=========================================================
	car action
=========================================================*/

/**
 * @fn static void Car_Wheels_Turn_Forward(void)
 * @brief ���ַ�����ǰ��
 */
static void Car_Wheels_Turn_Forward(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Forward);  // wheel11
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Forward);  // wheel12
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Forward);  // wheel21
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Forward);  // wheel22
	BLE_Send_String((uint8_t*)"turn forward");
}

/**
 * @fn static void Car_Wheels_Turn_Backward(void)
 * @brief ���ַ����ɺ���
 */
static void Car_Wheels_Turn_Backward(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Reverse);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Reverse);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Reverse);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Reverse);
	BLE_Send_String((uint8_t*)"turn backward");
}

/**
 * @fn static void Car_Wheels_TurnLeft(void)
 * @brief ���ַ�������ת
 */
static void Car_Wheels_TurnLeft(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Reverse);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Forward);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Reverse);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Forward);
	BLE_Send_String((uint8_t*)"turn left");
}

/**
 * @fn static void Car_Wheels_TurnRight(void)
 * @brief ���ַ�������ת
 */
static void Car_Wheels_TurnRight(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Forward);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Reverse);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Forward);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Reverse);
	BLE_Send_String((uint8_t*)"turn right");
}

/**
 * @fn static void Car_Update_All_Wheel_Duty(uint8_t duty)
 * @brief �������г��ֵ�ռ�ձ�
 */
static void Car_Update_All_Wheel_Duty(uint8_t duty)
{
	wheels[0].duty = duty;
	wheels[1].duty = duty;
	wheels[2].duty = duty;
	wheels[3].duty = duty;
	wheels[0].Update_Duty(&wheels[0]);
	wheels[1].Update_Duty(&wheels[1]);
	wheels[2].Update_Duty(&wheels[2]);
	wheels[3].Update_Duty(&wheels[3]);
}

/**
 * @fn static void Car_Move(void)
 * @brief ����ʼ�ƶ�
 */
static void Car_Move(void)
{
	Car_Update_All_Wheel_Duty(0);
	// car move flag
	BLE_Send_String((uint8_t*)"car is moving!");
}

/**
 * @fn static void Car_Stop(void)
 * @brief ��ֹͣ�ƶ�
 */
static void Car_Stop(void)
{
	Car_Update_All_Wheel_Duty(0);
	// car move flag
	BLE_Send_String((uint8_t*)"car has stopped!");
}

/**
 * @fn static void Car_Up_Update_AllWheel_Duty(void)
 * @brief �������������г��ֵ�ռ�ձȣ������ֹͣ����ֻ�������г��ֵ�ռ�ձ�
 */
static void Car_Up_Update_AllWheel_Duty(void)
{
	;
	BLE_Send_String((uint8_t*)"wheels' duty is: ");
	BLE_Send_Num(1);
}
/**
 * @fn static void Car_Low_Update_AllWheel_Duty(void)
 * @brief ���Ͳ��������г��ֵ�ռ�ձȣ������ֹͣ����ֻ�������г��ֵ�ռ�ձ�
 */
static void Car_Low_Update_AllWheel_Duty(void)
{
	;
	BLE_Send_String((uint8_t*)"wheels' duty is: ");
	BLE_Send_Num(1);
}

/**
 * @fn static void Car_Up_AllDuty_ChangeStep(void)
 * @brief �������г��ֵ�ռ�ձȱ仯����
 */
static void Car_Up_AllDuty_ChangeStep(void)
{
	;
	BLE_Send_String((uint8_t*)"wheels' duty change step is: ");
	BLE_Send_Num(1);
}

/**
 * @fn static void Car_Low_AllDuty_ChangeStep(void)
 * @brief �������г��ֵ�ռ�ձȱ仯����
 */
static void Car_Low_AllDuty_ChangeStep(void)
{
	;
	BLE_Send_String((uint8_t*)"wheels' duty change step is: ");
	BLE_Send_Num(1);
}

/*=========================================================
	car data packet decode and handle
=========================================================*/

/**
 * @fn void Car_BLE_Get_DataPacket_Rx(Car_DataPacket_Rx_t *dpr)
 * @brief �ӽ������ݻ������л�ȡһ�����ݣ������ý��ձ�־λUSART3_RX_STA
 *
 * @param [Car_DataPacket_Rx_t] һ֡���ݰ�ָ��
 */
void Car_BLE_Get_DataPacket_Rx(Car_DataPacket_Rx_t *dpr)
{
	uint8_t *pRxBuf = USART3_RX_BUF;
	
	if (USART3_RX_STA&USART3_RX_STA_REC_END) {  // �������һ������
		// ��λ��ͷ
		while (*pRxBuf != CAR_DATA_PACKET_HEAD_DEFAULT) pRxBuf++;
		dpr->packet_Head = *pRxBuf; pRxBuf++;
		// ԭʼ����
		dpr->rawData.flag = *pRxBuf; pRxBuf++;
		dpr->rawData.up_down = *pRxBuf; pRxBuf++;
		dpr->rawData.left_right = *pRxBuf; pRxBuf++;
		// У���
		dpr->check_Byte = *pRxBuf; pRxBuf++;
		// ��β����
		if (*pRxBuf == CAR_DATA_PACKET_TAIL_DEFAULT) dpr->packet_Tail = *pRxBuf;
		else dpr->packet_Tail = CAR_DATA_PACKET_TAIL_ERROR;
		
		// ���ý��ձ�־λ
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
	}
}

/**
 * @fn static void Car_BLE_DataPacket_Rx_Decode(Car_DataPacket_Rx_t dpr)
 * @brief ������ȷ�Ľ������ݰ�
 */
static void Car_BLE_DataPacket_Rx_Decode(Car_DataPacket_Rx_t dpr)
{
	;
}

/**
 * @fn void Car_DataPacket_Rx_Handle(Car_DataPacket_Rx_t dpr)
 * @brief �������ݰ�������
 */
void Car_DataPacket_Rx_Handle(Car_DataPacket_Rx_t dpr)
{
	uint8_t temp;
	
	temp = dpr.rawData.flag + dpr.rawData.up_down + dpr.rawData.left_right;
	// �������ݴ���
	if (temp != dpr.check_Byte) {
		printf("Car BLE rec data packet error!\r\n");
		return;
	}
	// ����������ȷ
	Car_BLE_DataPacket_Rx_Decode(dpr);
}

/*=========================================================
	car initialization
=========================================================*/

/**
 * @fn static void Car_Wheels_Init(void)
 * @brief ��ʼ������
 */
static void Car_Wheels_Init(void)
{
	Motor_GPIO_Config(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_14|GPIO_Pin_15);	// wheel11
	Motor_GPIO_Config(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_1|GPIO_Pin_2);		// wheel12
	Motor_GPIO_Config(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_5|GPIO_Pin_4);		// wheel21
	Motor_GPIO_Config(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_7|GPIO_Pin_6);		// wheel22
}

/**
 * @fn static void Car_Wheels_Pwm_Init(void)
 * @brief ��ʼ�����ֵ�PWM
 */
static void Car_Wheels_Pwm_Init(void)
{
	Timer1_Pwm_Init(899, 0);  // wheel11, 72M/900=80kHz
	Timer2_Pwm_Init(899, 0);  // wheel12, 72M/900=80kHz
	Timer3_Pwm_Init(899, 0);  // wheel21, 72M/900=80kHz
	Timer4_Pwm_Init(899, 0);  // wheel22, 72M/900=80kHz
}

/**
 * @fn void Car_Init(void)
 * @brief С����ʼ��
 */
void Car_Init(void)
{
	Car_Wheels_Init();
	BLE_Init();
	Car_Wheels_Pwm_Init();
	
	Car_Stop();
	Car_Wheels_Turn_Forward();
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("             Car has been initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
}
