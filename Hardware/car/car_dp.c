#include "car_dp.h"

#include <string.h>

#include "usart.h"
#include "car.h"

/*=========================================================
	��Ҫ�Ա�ģ�������ı���
=========================================================*/

extern uint8_t allW_duty;
extern uint8_t allW_duty_changeStep;
extern Motor_t wheels[4];

/*=========================================================
	С���������ݰ�FIFO
=========================================================*/

Car_DataPacket_Rx_t dpr[1];

/*=========================================================
	car data packet send and get
=========================================================*/

/**
 * @fn void Car_Send_DataPacket_Tx(void)
 * @brief ����λ������С����Ϣ
 */
void Car_Send_DataPacket_Tx(void)
{
	Car_DataPacket_Tx_t dpt;
	
	dpt.packet_Head = CAR_DATAPACKET_HEAD_DEFAULT;
	dpt.rawData.allW_duty = allW_duty;
	dpt.rawData.w11_duty = wheels[0].duty;
	dpt.rawData.w12_duty = wheels[1].duty;
	dpt.rawData.w21_duty = wheels[2].duty;
	dpt.rawData.w22_duty = wheels[3].duty;
	dpt.rawData.allW_duty_changeStep = allW_duty_changeStep;
	dpt.check_Byte =\
		dpt.rawData.allW_duty +\
		dpt.rawData.w11_duty + dpt.rawData.w12_duty +\
		dpt.rawData.w21_duty + dpt.rawData.w22_duty +\
		dpt.rawData.allW_duty_changeStep;
	dpt.packet_Tail = CAR_DATAPACKET_TAIL_DEFAULT;
	
	USART3_Send_Char(dpt.packet_Head);
	USART3_Send_Char(dpt.rawData.allW_duty);
	USART3_Send_Char(dpt.rawData.w11_duty);
	USART3_Send_Char(dpt.rawData.w12_duty);
	USART3_Send_Char(dpt.rawData.w21_duty);
	USART3_Send_Char(dpt.rawData.w22_duty);
	USART3_Send_Char(dpt.rawData.allW_duty_changeStep);
	USART3_Send_Char(dpt.check_Byte);
	USART3_Send_Char(dpt.packet_Tail);
}

/**
 * @fn void Car_Get_DataPacket_Rx(void)
 * @brief �ӽ������ݻ������л�ȡһ�����ݣ������ý��ձ�־λUSART3_RX_STA
 */
void Car_Get_DataPacket_Rx(void)
{
	uint8_t *pRxBuf = USART3_RX_BUF;
	
	if (USART3_RX_STA&USART3_RX_STA_REC_END) {  // �������һ������
		// ��λ��ͷ
		while (*pRxBuf != CAR_DATAPACKET_HEAD_DEFAULT) pRxBuf++;
		dpr[0].packet_Head = *pRxBuf; pRxBuf++;
		// ԭʼ����
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
