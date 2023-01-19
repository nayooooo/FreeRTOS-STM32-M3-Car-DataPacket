#include "car.h"

#include <stdio.h>
#include <string.h>

/*=========================================================
	static car flag
=========================================================*/

// �������ݰ��ı�־λ����Car_BLE_Get_DataPacket_Rx��ˢ��
static uint8_t rxflag = CAR_DATAPACKET_RX_FLAG_NULL;

/*=========================================================
	static car state
=========================================================*/

static uint8_t allW_duty = CAR_WHEELS_DUTY_MIN;
static uint8_t allW_duty_changeStep = CAR_WHEELS_DUTY_CHANGE_STEP_MIN;

/*=========================================================
	extern car action
=========================================================*/

static void Car_Set_Wheels_Dir_Duty(void);
static void Car_Move(void);
static void Car_Stop(void);
static void Car_Up_AllWheel_Duty(void);
static void Car_Low_AllWheel_Duty(void);
static void Car_Up_AllDuty_ChangeStep(void);
static void Car_Low_AllDuty_ChangeStep(void);

/*=========================================================
	С��״̬���б�
=========================================================*/

static Car_StateMachine_t Car_StateMachine_Table[] = {
	{ CAR_STATEMACHINE_STOP, Car_Stop },										// С��ֹͣ
	{ CAR_STATEMACHINE_SETACTION, NULL },										// С�����ö���
	{ CAR_STATEMACHINE_UP_ALLDUTY, Car_Up_AllWheel_Duty },						// ����С�����г���ռ�ձ�
	{ CAR_STATEMACHINE_LOW_ALLDUTY, Car_Low_AllWheel_Duty },					// ����С�����г���ռ�ձ�
	{ CAR_STATEMACHINE_UP_ALLDUTY_CHANGESTEP, Car_Up_AllDuty_ChangeStep },		// ����С�����г���ռ�ձȱ仯����
	{ CAR_STATEMACHINE_LOW_ALLDUTY_CHANGESTEP, Car_Low_AllDuty_ChangeStep },	// ����С�����г���ռ�ձȱ仯����
	{ CAR_STATEMACHINE_TURNUP, Car_Set_Wheels_Dir_Duty },						// С��ǰ��
	{ CAR_STATEMACHINE_TURNDOWN, Car_Set_Wheels_Dir_Duty },						// С������
	{ CAR_STATEMACHINE_TURNLEFT, Car_Set_Wheels_Dir_Duty },						// С����ת
	{ CAR_STATEMACHINE_TURNRIGHT, Car_Set_Wheels_Dir_Duty },					// С����ת
	{ CAR_STATEMACHINE_NULL, NULL },											// ״̬���б�ĩβ
};

/*=========================================================
	С���������ݰ�FIFO
=========================================================*/

Car_DataPacket_Rx_t dpr[1];

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
 * @fn static void Car_Wheels_Dir_Turn_Forward(void)
 * @brief ���ַ�����ǰ��
 */
static void Car_Wheels_Dir_Turn_Forward(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Forward);  // wheel11
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Forward);  // wheel12
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Forward);  // wheel21
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Forward);  // wheel22
	BLE_Send_String((uint8_t*)"turn forward");
}

/**
 * @fn static void Car_Wheels_Dir_Turn_Backward(void)
 * @brief ���ַ����ɺ���
 */
static void Car_Wheels_Dir_Turn_Backward(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Reverse);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Reverse);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Reverse);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Reverse);
	BLE_Send_String((uint8_t*)"turn backward");
}

/**
 * @fn static void Car_Wheels_Dir_TurnLeft(void)
 * @brief ���ַ�������ת
 */
static void Car_Wheels_Dir_TurnLeft(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Reverse);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Forward);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Reverse);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Forward);
	BLE_Send_String((uint8_t*)"turn left");
}

/**
 * @fn static void Car_Wheels_Dir_TurnRight(void)
 * @brief ���ַ�������ת
 */
static void Car_Wheels_Dir_TurnRight(void)
{
	wheels[0].Set_Dir(&wheels[0], Motor_Rotate_Dir_Forward);
	wheels[1].Set_Dir(&wheels[1], Motor_Rotate_Dir_Reverse);
	wheels[2].Set_Dir(&wheels[2], Motor_Rotate_Dir_Forward);
	wheels[3].Set_Dir(&wheels[3], Motor_Rotate_Dir_Reverse);
	BLE_Send_String((uint8_t*)"turn right");
}

/**
 * @fn static void (void)
 * @brief ȷ��С���������ֵķ����ռ�ձ�
 */
static void Car_Set_Wheels_Dir_Duty(void)
{
	// �ж��ƶ�����
	if (dpr[0].rawData.up_down > 0) {  // up
	} else if (dpr[0].rawData.up_down < 0) {  // down
	}
	// �������趨��ɣ���ʼ���³���
	// ���û�н���˺�����˵��ң����Ĭ��״̬������Ҫ�ƶ�
	Car_Move();
}

/**
 * @fn static void Car_Update_All_Wheel_Duty_Same(uint8_t duty)
 * @brief �������г��ֵ�ռ�ձȣ�ռ�ձ�һ�£�
 */
static void Car_Update_All_Wheel_Duty_Same(uint8_t duty)
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
 * @fn static void Car_Update_All_Wheel_Duty(void)
 * @brief �������г��ֵ�ռ�ձ�
 */
static void Car_Update_All_Wheel_Duty(void)
{
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
	if (!(rxflag&CAR_DATAPACKET_RX_FLAG_ISSTOP)) {
		Car_Update_All_Wheel_Duty_Same(allW_duty);
		// car move flag
		BLE_Send_String((uint8_t*)"car has moving!");
	} else {
		BLE_Send_String((uint8_t*)"Don't move it!\r\n");
	}
}

/**
 * @fn static void Car_Stop(void)
 * @brief ��ֹͣ�ƶ�
 */
static void Car_Stop(void)
{
	if (rxflag&CAR_DATAPACKET_RX_FLAG_ISSTOP) {
		Car_Update_All_Wheel_Duty_Same(0);
		// car move flag
		BLE_Send_String((uint8_t*)"car has stopped!");
	}
}

/**
 * @fn static void Car_Up_AllWheel_Duty(void)
 * @brief �������г��ֵ�ռ�ձ�
 */
static void Car_Up_AllWheel_Duty(void)
{
	allW_duty += allW_duty_changeStep;
	if ((allW_duty > CAR_WHEELS_DUTY_MAX) ||\
		(allW_duty < CAR_WHEELS_DUTY_MIN))
		allW_duty -= allW_duty_changeStep;
	BLE_Send_String((uint8_t*)"all wheels' duty is: ");
	BLE_Send_Num(allW_duty);
}
/**
 * @fn static void Car_Low_AllWheel_Duty(void)
 * @brief �������г��ֵ�ռ�ձ�
 */
static void Car_Low_AllWheel_Duty(void)
{
	allW_duty -= allW_duty_changeStep;
	if ((allW_duty > CAR_WHEELS_DUTY_MAX) ||\
		(allW_duty < CAR_WHEELS_DUTY_MIN))
		allW_duty += allW_duty_changeStep;
	BLE_Send_String((uint8_t*)"all wheels' duty is: ");
	BLE_Send_Num(allW_duty);
}

/**
 * @fn static void Car_Up_AllDuty_ChangeStep(void)
 * @brief �������г��ֵ�ռ�ձȱ仯����
 */
static void Car_Up_AllDuty_ChangeStep(void)
{
	allW_duty_changeStep++;
	if ((allW_duty_changeStep > CAR_WHEELS_DUTY_CHANGE_STEP_MAX) ||\
		(allW_duty_changeStep < CAR_WHEELS_DUTY_CHANGE_STEP_MIN))
		allW_duty_changeStep--;
	BLE_Send_String((uint8_t*)"all wheels' duty change step is: ");
	BLE_Send_Num(allW_duty_changeStep);
}

/**
 * @fn static void Car_Low_AllDuty_ChangeStep(void)
 * @brief �������г��ֵ�ռ�ձȱ仯����
 */
static void Car_Low_AllDuty_ChangeStep(void)
{
	allW_duty_changeStep--;
	if ((allW_duty_changeStep > CAR_WHEELS_DUTY_CHANGE_STEP_MAX) ||\
		(allW_duty_changeStep < CAR_WHEELS_DUTY_CHANGE_STEP_MIN))
		allW_duty_changeStep++;
	BLE_Send_String((uint8_t*)"all wheels' duty change step is: ");
	BLE_Send_Num(allW_duty_changeStep);
}

/*=========================================================
	car data packet decode and handle
=========================================================*/

/**
 * @fn void Car_BLE_Get_DataPacket_Rx(void)
 * @brief �ӽ������ݻ������л�ȡһ�����ݣ������ý��ձ�־λUSART3_RX_STA
 */
void Car_BLE_Get_DataPacket_Rx(void)
{
	uint8_t *pRxBuf = USART3_RX_BUF;
	
	if (USART3_RX_STA&USART3_RX_STA_REC_END) {  // �������һ������
		// ��λ��ͷ
		while (*pRxBuf != CAR_DATAPACKET_HEAD_DEFAULT) pRxBuf++;
		dpr[0].packet_Head = *pRxBuf; pRxBuf++;
		// ԭʼ����
		rxflag = *pRxBuf;
		dpr[0].rawData.flag = *pRxBuf; pRxBuf++;
		dpr[0].rawData.up_down = *pRxBuf; pRxBuf++;
		dpr[0].rawData.left_right = *pRxBuf; pRxBuf++;
		// У���
		dpr[0].check_Byte = *pRxBuf; pRxBuf++;
		// ��β����
		if (*pRxBuf == CAR_DATAPACKET_TAIL_DEFAULT) dpr[0].packet_Tail = *pRxBuf;
		else dpr[0].packet_Tail = CAR_DATAPACKET_TAIL_ERROR;
		
		// ���ý��ձ�־λ
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
	}
}

/**
 * @fn static Car_BLE_StateMachine_Event_Arr_t *Car_BLE_DataPacket_Rx_Decode(Car_DataPacket_Rx_t dpr)
 * @brief ����һ֡��ȷ�Ľ������ݰ��������¼���ṹ�����ʽ���ؽ�����Ϣ
 * @details �ú���������������ݰ�����ȷ�ԣ��뿪����Ա�������ݰ�֮ǰ���һ��
 *
 * @return [Car_BLE_StateMachine_Event_Arr_t*] С���¼��б��¼���Ŀ
 */
static Car_BLE_StateMachine_Event_Arr_t *Car_BLE_DataPacket_Rx_Decode(Car_DataPacket_Rx_t dpr)
{
	static Car_BLE_StateMachine_Event_Arr_t event;
	int8_t ind = 0;
	int8_t temp;
	
	// ��ʼ���¼���
	event.num = 0;
	memset(event.events, 0, sizeof(event.events));
	
	// ����
	/* ��־λ */
	for (temp = 0X01;\
		temp <= CAR_DATAPACKET_RX_FLAG_LOW_ALLDUTY_CHANGESTEP; temp <<= 1) {
		if (dpr.rawData.flag&temp) {
			event.events[ind] = temp;
			event.num++;
			ind++;
		}
	}
	/* ���� */
	/* up_down upΪ�� */
	if (dpr.rawData.up_down > 0) {  // up
		event.events[ind] = CAR_STATEMACHINE_TURNUP;
		event.num++;
		ind++;
	} else if (dpr.rawData.up_down < 0) {  // down
		event.events[ind] = CAR_STATEMACHINE_TURNDOWN;
		event.num++;
		ind++;
	}  // up_down == 0 �������¼���
	/* left_right rightΪ�� */
	if (dpr.rawData.left_right > 0) {  // right
		event.events[ind] = CAR_STATEMACHINE_TURNRIGHT;
		event.num++;
		ind++;
	} else if (dpr.rawData.left_right < 0) {  // left
		event.events[ind] = CAR_STATEMACHINE_TURNLEFT;
		event.num++;
		ind++;
	}  // left_right == 0 �������¼���
	
	return &event;
}

/**
 * @fn void Car_DataPacket_Rx_Handle(void)
 * @brief �������ݰ�������
 */
void Car_DataPacket_Rx_Handle(void)
{
	uint8_t i, j;
	uint8_t temp;
	Car_BLE_StateMachine_Event_Arr_t *event;
	
	temp = dpr[0].rawData.flag + dpr[0].rawData.up_down + dpr[0].rawData.left_right;
	// �������ݴ���
	if ((dpr[0].packet_Tail == CAR_DATAPACKET_TAIL_ERROR) || (temp != dpr[0].check_Byte)) {
		printf("Car BLE rec data packet error!\r\n");
		return;
	}
	// ����������ȷ
	event = Car_BLE_DataPacket_Rx_Decode(dpr[0]);  // �����ȡ�¼���
	for (i = 0; i < event->num; i++) {
		for (j = 0; Car_StateMachine_Table[j].event; j++) {  // ����״̬���б�
			if (event->events[i] == Car_StateMachine_Table[j].event)
				Car_StateMachine_Table[j].act();
		}
	}
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
	Car_Wheels_Dir_Turn_Forward();
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("             Car has been initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
}
