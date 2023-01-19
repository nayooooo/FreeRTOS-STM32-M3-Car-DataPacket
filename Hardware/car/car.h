#ifndef __CAR_H
#define __CAR_H

#include "sys.h"

#include "timer.h"
#include "motor.h"
#include "ble.h"
#include "car_dp.h"

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

/* car state machine ------------------------------------*/

// С���¼�ö��
typedef enum{
	CAR_STATEMACHINE_NULL,						// С�����¼�
	CAR_STATEMACHINE_STOP,						// С��ֹͣ
	CAR_STATEMACHINE_SETACTION,					// С�����ö���
	CAR_STATEMACHINE_UP_ALLDUTY,				// ����С�����г���ռ�ձ�
	CAR_STATEMACHINE_LOW_ALLDUTY,				// ����С�����г���ռ�ձ�
	CAR_STATEMACHINE_UP_ALLDUTY_CHANGESTEP,		// ����С�����г���ռ�ձȱ仯����
	CAR_STATEMACHINE_LOW_ALLDUTY_CHANGESTEP,	// ����С�����г���ռ�ձȱ仯����
	CAR_STATEMACHINE_TURNUP,					// С��ǰ��
	CAR_STATEMACHINE_TURNDOWN,					// С������
	CAR_STATEMACHINE_TURNLEFT,					// С����ת
	CAR_STATEMACHINE_TURNRIGHT,					// С����ת
	CAR_STATEMACHINE_ROTATRLEFT,				// С��ԭ��������ת
	CAR_STATEMACHINE_ROTATRRIGHT,				// С��ԭ��������ת
}Car_StateMachine_Event_Enum_t;
typedef struct Car_StateMachine_Event_Arr{
	uint8_t num;			// ���ν�����¼���Ŀ
	// ���ν�����¼��б�
	Car_StateMachine_Event_Enum_t events[\
		CAR_STATEMACHINE_FLAG_EVENT + CAR_STATEMACHINE_VARI_EVENT];
}Car_StateMachine_Event_Arr_t;
typedef void(*Car_StateMachine_Action)(void);
typedef struct{
	Car_StateMachine_Event_Enum_t event;		// С���¼�
	Car_StateMachine_Action act;					// С������
}Car_StateMachine_t;

/* car functions ----------------------------------------*/

void Car_Init(void);

void Car_DataPacket_Rx_Handle(void);

#endif /* __CAR_H */
