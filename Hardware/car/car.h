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

// 小车事件枚举
typedef enum{
	CAR_STATEMACHINE_NULL,						// 小车无事件
	CAR_STATEMACHINE_STOP,						// 小车停止
	CAR_STATEMACHINE_SETACTION,					// 小车设置动作
	CAR_STATEMACHINE_UP_ALLDUTY,				// 增加小车所有车轮占空比
	CAR_STATEMACHINE_LOW_ALLDUTY,				// 降低小车所有车轮占空比
	CAR_STATEMACHINE_UP_ALLDUTY_CHANGESTEP,		// 增加小车所有车轮占空比变化步长
	CAR_STATEMACHINE_LOW_ALLDUTY_CHANGESTEP,	// 降低小车所有车轮占空比变化步长
	CAR_STATEMACHINE_TURNUP,					// 小车前进
	CAR_STATEMACHINE_TURNDOWN,					// 小车后退
	CAR_STATEMACHINE_TURNLEFT,					// 小车左转
	CAR_STATEMACHINE_TURNRIGHT,					// 小车右转
	CAR_STATEMACHINE_ROTATRLEFT,				// 小车原地向左旋转
	CAR_STATEMACHINE_ROTATRRIGHT,				// 小车原地向右旋转
}Car_StateMachine_Event_Enum_t;
typedef struct Car_StateMachine_Event_Arr{
	uint8_t num;			// 本次解码的事件数目
	// 本次解码的事件列表
	Car_StateMachine_Event_Enum_t events[\
		CAR_STATEMACHINE_FLAG_EVENT + CAR_STATEMACHINE_VARI_EVENT];
}Car_StateMachine_Event_Arr_t;
typedef void(*Car_StateMachine_Action)(void);
typedef struct{
	Car_StateMachine_Event_Enum_t event;		// 小车事件
	Car_StateMachine_Action act;					// 小车动作
}Car_StateMachine_t;

/* car functions ----------------------------------------*/

void Car_Init(void);

void Car_DataPacket_Rx_Handle(void);

#endif /* __CAR_H */
