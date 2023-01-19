#ifndef __CAR_DP_H
#define __CAR_DP_H

#include "sys.h"

#define CAR_DATAPACKET_RX_FLAG_NULL						(0X00)
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
	uint8_t allW_duty;
	uint8_t w11_duty;
	uint8_t w12_duty;
	uint8_t w21_duty;
	uint8_t w22_duty;
	uint8_t allW_duty_changeStep;
}Car_DataPacket_Tx_RawData_t;
typedef struct Car_DataPacket_Tx{
	int8_t packet_Head;
	Car_DataPacket_Tx_RawData_t rawData;
	int8_t check_Byte;
	int8_t packet_Tail;
}Car_DataPacket_Tx_t;

/* 小车接收数据包FIFO */

extern Car_DataPacket_Rx_t dpr[1];

/* car data packet --------------------------------------*/

#define CAR_DATAPACKET_HEAD_DEFAULT		(0XA5)
#define CAR_DATAPACKET_TAIL_DEFAULT		(0X5A)
#define CAR_DATAPACKET_TAIL_ERROR		(0X00)

/* car state machine ------------------------------------*/

#define CAR_STATEMACHINE_FLAG_EVENT			(4)		// 标志位事件数目
#define CAR_STATEMACHINE_VARI_EVENT			(2)		// 变量事件数目

/* car functions ----------------------------------------*/

void Car_Get_DataPacket_Rx(void);
void Car_Send_DataPacket_Tx(void);

#endif /* __CAR_DP_H */
