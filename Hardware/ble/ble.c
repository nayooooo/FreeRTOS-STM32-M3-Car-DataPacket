#include "ble.h"

void BLE_Init(void)
{
	USART3_Init(115200);
}

void BLE_Send_Num(uint16_t num)
{
	USART3_Send_Num(num);
}

void BLE_Send_Char(uint8_t chr)
{
	USART3_Send_Char(chr);
}

void BLE_Send_String(uint8_t *str)
{
	USART3_Send_String(str);
}

uint8_t BLE_Get_Char(uint16_t addr)
{
	return USART3_Get_Char(addr);
}

uint8_t *BLE_Get_String(void)
{
	return USART3_Get_String();
}

void BLE_Postback(void)
{
	USART3_Postback();
}
