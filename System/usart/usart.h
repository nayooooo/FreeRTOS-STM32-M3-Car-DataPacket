#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��

/*===================================================
	USART1
===================================================*/

#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 				1		//ʹ�ܣ�1��/��ֹ��0������1����
extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART1_RX_STA;					//����״̬���

void USART1_Init(uint32_t bound);
void USART1_Send_String(uint8_t *str);
void USART1_Postback(void);

/*===================================================
	USART3
===================================================*/

#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 				1		//ʹ�ܣ�1��/��ֹ��0������1����
extern uint8_t  USART3_RX_BUF[USART3_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
#define USART3_RX_STA_REC_END			(0X8000)		// bit15		�������һ������
#define USART3_RX_STA_HAS_REC			(0X4000)		// bit14		�н���
#define USART3_RX_STA_OVERFLOW			(0X3000)		// bit13~12		�������������
#define USART3_RX_STA_REC_NUM			(0X0FFF)		// bit11~0		�����������ַ���
extern uint16_t USART3_RX_STA;					//����״̬���

void USART3_Init(uint32_t bound);
void USART3_Send_Char(uint8_t chr);
void USART3_Send_Num(uint16_t num);
void USART3_Send_String(uint8_t *str);
uint8_t USART3_Get_Char(uint16_t addr);
uint8_t *USART3_Get_String(void);
void USART3_Postback(void);
#endif


