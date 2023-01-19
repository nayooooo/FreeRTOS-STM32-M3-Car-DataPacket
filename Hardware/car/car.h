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

#define CAR_DATA_PACKET_HEAD_DEFAULT		(0XA5)
#define CAR_DATA_PACKET_TAIL_DEFAULT		(0X5A)
#define CAR_DATA_PACKET_TAIL_ERROR			(0X00)

/* Rx */
typedef struct Car_DataPacket_Rx_RawData{
	// bit5		->	low_AllDuty_ChangeStep
	// bit4		->	up_AllDuty_ChangeStep
	// bit3		->	low_AllDuty
	// bit2		->	up_AllDuty
	// bit1		->	isSetAction
	// bit0		->	isStop
	int8_t flag;
	// up_down和left_right的取值域为[-100,100]
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

typedef void(*Car_StateMachine_Action)(void);
typedef struct{
	;
	Car_StateMachine_Action act;
}Car_StateMachine_t;

/* car functions ----------------------------------------*/

void Car_Init(void);

void Car_BLE_Get_DataPacket_Rx(Car_DataPacket_Rx_t *dpr);
void Car_DataPacket_Rx_Handle(Car_DataPacket_Rx_t dpr);

#endif /* __CAR_H */
