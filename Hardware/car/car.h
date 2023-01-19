#ifndef __CAR_H
#define __CAR_H

#include "sys.h"

#include "motor.h"
#include "ble.h"
#include "timer.h"

/* car motor pin micro ----------------------------------*/

#define Motor11_A PBout(4)
#define Motor11_B PBout(3)

#define Motor12_A PAout(15)
#define Motor12_B PAout(12)

#define Motor21_A PAout(5)
#define Motor21_B PAout(4)

#define Motor22_A PAout(7)
#define Motor22_B PAout(6)

/* car motor duty micro ---------------------------------*/

#define CAR_WHEELS_DUTY_MAX		(100)
#define CAR_WHEELS_DUTY_MIN		(70)

#define CAR_WHEELS_DUTY_CHANGE_STEP_MAX			(5)
#define CAR_WHEELS_DUTY_CHANGE_STEP_MIN			(1)

/* car motors -------------------------------------------*/

extern Motor_t motors[4];

/* car data packet --------------------------------------*/

#define CAR_DATAPACKET_HEAD_DEFAULT		(0XA5)
#define CAR_DATAPACKET_TAIL_DEFAULT		(0X5A)
#define CAR_DATAPACKET_TAIL_ERROR		(0X00)

#define CAR_DATAPACKET_RX_FLAG_ISSTOP					(0X01)
#define CAR_DATAPACKET_RX_FLAG_ISSETACTION				(0X02)
#define CAR_DATAPACKET_RX_FLAG_UP_ALLDUTY				(0X04)
#define CAR_DATAPACKET_RX_FLAG_LOW_ALLDUTY				(0X08)
#define CAR_DATAPACKET_RX_FLAG_UP_ALLDUTY_CHANGESTEP	(0X10)
#define CAR_DATAPACKET_RX_FLAG_LOW_ALLDUTY_CHANGESTEP	(0X20)
/* Rx */
typedef struct Car_DataPacket_Rx_RawData{
	// bit5		->	low_AllDuty_ChangeStep
	// bit4		->	up_AllDuty_ChangeStep
	// bit3		->	low_AllDuty
	// bit2		->	up_AllDuty
	// bit1		->	isSetAction
	// bit0		->	isStop
	int8_t flag;
	// up_down��left_right��ȡֵ��Ϊ[-100,100]
	int8_t up_down;
	int8_t left_right;
}Car_DataPacket_Rx_RawData_t;
typedef struct Car_DataPacket_Rx{
	int8_t packet_Head;
	Car_DataPacket_Rx_RawData_t rawData;
	int8_t check_Byte;
	int8_t packet_Tail;
}Car_DataPacket_Rx_t;

/* Tx */
typedef struct Car_DataPacket_Tx_RawData{
	int16_t w11_duty;
	int16_t w12_duty;
	int16_t w21_duty;
	int16_t w22_duty;
	int16_t allW_duty;
}Car_DataPacket_Tx_RawData_t;
typedef struct Car_DataPacket_Tx{
	int8_t packet_Head;
	Car_DataPacket_Tx_RawData_t rawData;
	int8_t check_Byte;
	int8_t packet_Tail;
}Car_DataPacket_Tx_t;

/* car state machine ------------------------------------*/

#define CAR_BLE_STATEMACHINE_FLAG_EVENT			(4)		// ��־λ�¼���Ŀ
#define CAR_BLE_STATEMACHINE_VARI_EVENT			(2)		// �����¼���Ŀ
// С��״̬ö��
typedef enum{
	CAR_BLE_STATEMACHINE_STOP		= 1,			// С��ֹͣ״̬
	CAR_BLE_STATEMACHINE_SETACTION,					// С�����ö���
	CAR_BLE_STATEMACHINE_UP_ALLDUTY,				// ����С�����г���ռ�ձ�
	CAR_BLE_STATEMACHINE_LOW_ALLDUTY,				// ����С�����г���ռ�ձ�
	CAR_BLE_STATEMACHINE_UP_ALLDUTY_CHANGESTEP,		// ����С�����г���ռ�ձȱ仯����
	CAR_BLE_STATEMACHINE_LOW_ALLDUTY_CHANGESTEP,	// ����С�����г���ռ�ձȱ仯����
	CAR_BLE_STATEMACHINE_TURNUP,					// С��ǰ��
	CAR_BLE_STATEMACHINE_TURNDOWN,					// С������
	CAR_BLE_STATEMACHINE_TURNLEFT,					// С����ת
	CAR_BLE_STATEMACHINE_TURNRIGHT,					// С����ת
}Car_BLE_StateMachine_Event_Enum_t;
typedef struct{
	uint8_t num;			// ���ν�����¼���Ŀ
	// ���ν�����¼��б�
	Car_BLE_StateMachine_Event_Enum_t events[\
		CAR_BLE_STATEMACHINE_FLAG_EVENT + CAR_BLE_STATEMACHINE_VARI_EVENT];
}Car_BLE_StateMachine_Event_Arr_t;
typedef void(*Car_StateMachine_Action)(void);
typedef struct{
	Car_BLE_StateMachine_Event_Enum_t event;		// С���¼�
	Car_StateMachine_Action act;					// С������
}Car_StateMachine_t;

/* car functions ----------------------------------------*/

void Car_Init(void);

void Car_BLE_Get_DataPacket_Rx(Car_DataPacket_Rx_t *dpr);
void Car_DataPacket_Rx_Handle(Car_DataPacket_Rx_t dpr);

#endif /* __CAR_H */
