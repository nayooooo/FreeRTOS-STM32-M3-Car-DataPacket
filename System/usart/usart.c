#include <string.h>
#include "usart.h"	  
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif
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
////////////////////////////////////////////////////////////////////////////////// 	  
 

////////////////////////////////////////////////////////////////// 
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
//#pragma import(__use_no_semihosting)             
__asm (".global __use_no_semihosting\n\t");
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&USART_FLAG_TC)==0){}//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //���ʹ���˽���
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART1_RX_STA=0;       //����״̬���	  
  
void USART1_Init(uint32_t bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);		//��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    	//ʹ�ܴ���1
	
	memset(USART1_RX_BUF, 0, USART1_REC_LEN);
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("       STM32F103C8T6 USART1 has initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
}

void USART1_Send_String(uint8_t *str)
{
	uint8_t chr = 0;
	
	while(*str) {
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // �ȴ��������
		USART_SendData(USART1, *str);
		str++;
	}
	// �����ַ���ĩβ��'\0'
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // �ȴ��������
	USART_SendData(USART1, *str);
}

void USART1_Postback(void)
{
	if(USART1_RX_STA&0X8000) {  // ����������
		USART1_Send_String(USART1_RX_BUF);
		memset(USART1_RX_BUF, 0, USART1_REC_LEN);
		USART1_RX_STA = 0;
	}
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART1_RX_STA&0x8000)==0)//����δ���
			{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
     } 
} 
#endif /* EN_USART1_RX */

#if EN_USART3_RX   //���ʹ���˽���

#include <math.h>

/* ���������� */
typedef struct Car_DataPacket_Rx Car_DataPacket_Rx_t;
typedef struct Car_DataPacket_Tx Car_DataPacket_Tx_t;

uint8_t  USART3_RX_BUF[USART3_REC_LEN];
//����״̬
//bit15��	���յ�δ��ȡ(1)
//bit14��	�������(1)
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART3_RX_STA = 0;

static uint32_t USART_Pow(uint32_t base, uint32_t index)
{
	uint32_t res = 1;
	
	while (index--) res *= base;
	
	return res;
}

void USART3_Init(uint32_t bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
  
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11 

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 		//��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    	//ʹ�ܴ���3
	
	memset(USART3_RX_BUF, 0, USART3_REC_LEN);
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("       STM32F103C8T6 USART3 has initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
}

void USART3_Send_Char(uint8_t chr)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // �ȴ��������
	USART_SendData(USART3, chr);
}

void USART3_Send_Num(uint16_t num)
{
	uint8_t chr = 0;
	uint8_t digital = 0;
	uint16_t temp = num;
	
	do {
		digital++;
		temp /= 10;
	} while(temp);
	
	temp = num;
	while (digital--) {
		chr = (uint8_t)(temp / (uint16_t)USART_Pow(10, digital)) + '0';
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // �ȴ��������
		USART_SendData(USART3, chr);
		temp %= (uint16_t)USART_Pow(10, digital);
	}
}

void USART3_Send_String(uint8_t *str)
{
	uint8_t chr = 0;
	
	while(*str) {
		USART3_Send_Char(*str);
		str++;
	}
	// �����ַ���ĩβ��'\0'
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // �ȴ��������
	USART_SendData(USART3, *str);
}

/**
 * @fn uint8_t USART3_Get_Chr(uint16_t addr)
 * @brief ��ý��ջ����е�һ���ַ�
 *
 * @param [addr] ��ȡ���ַ����ڵ�λ��
 * @return [uint8_t} char
 */
uint8_t USART3_Get_Char(uint16_t addr)
{
	uint8_t chr = '\0';
	
	if(USART3_RX_STA&USART3_RX_STA_HAS_REC) {
		if(addr < USART3_REC_LEN) chr = USART3_RX_BUF[addr];
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
	}
	
	return chr;
}

/**
 * @fn uint8_t USART3_Get_String(uint16_t addr)
 * @brief ��ý��ջ��������ݣ��ַ�����ʽ��
 *=
 * @return [uint8_t*} string
 */
uint8_t *USART3_Get_String(void)
{
	if(USART3_RX_STA&USART3_RX_STA_HAS_REC) {
		USART3_RX_BUF[USART3_RX_STA&USART3_RX_STA_REC_NUM] = 0;
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
		return USART3_RX_BUF;
	} else {
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
		return (uint8_t*)"error\r\n";
	}
}

void USART3_Postback(void)
{
	if(USART3_RX_STA&USART3_RX_STA_HAS_REC) {  // ����������
		USART3_Send_String(USART3_RX_BUF);
		memset(USART3_RX_BUF, 0, USART3_REC_LEN);
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
	}
}

void USART3_IRQHandler(void)                	// ����3�жϷ������
{
	static uint8_t errTimes = 0;  // ���ݰ��������
	// ����С��������ݰ� Car_DataPacket_Rx_t �ı�־λ
	// bit2~0	->	���յ���ԭʼ���ݺ�У��͵��ֽ���
	// bit3		->	���յ��˰�ͷ����
	// bit4		->	��������ԭʼ����
	static uint8_t car_DataPacket_Rx_Flag = 0X00;
	uint8_t Res = 0;
	 
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {  // �����ж�
		Res =USART_ReceiveData(USART3);	// ��ȡ���յ�������
		USART3_RX_BUF[USART3_RX_STA&USART3_RX_STA_REC_NUM] = Res;
		USART3_RX_STA++;
		USART3_RX_BUF[USART3_RX_STA&USART3_RX_STA_REC_NUM] = 0;
		USART3_RX_STA |= USART3_RX_STA_HAS_REC;
		
		// �������ݰ�
		// ����δ���
		if (!(USART3_RX_STA&USART3_RX_STA_REC_END)) {
			if (car_DataPacket_Rx_Flag&0X08) {  // ���յ��˰�ͷ����
				// ����ԭʼ���ݺ�У���
				if ((car_DataPacket_Rx_Flag&0X10) || ((car_DataPacket_Rx_Flag&0X07) == 4)) {  // �������
					car_DataPacket_Rx_Flag |= 0X10;
					// ���հ�β����
					if (Res == 0X5A) {  // ���յ��˰�β����
						USART3_RX_STA |= USART3_RX_STA_REC_END;
					} else {  // ���ݰ�����ʧ��
						errTimes++;
						printf("BLE rec failed!\r\n");
						USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
					}
					car_DataPacket_Rx_Flag = 0X00;
				} else car_DataPacket_Rx_Flag++;
			} else {
				if (Res == 0XA5)  // ���ڽ��յ��ǰ�ͷ����
					car_DataPacket_Rx_Flag |= 0X08;
			}
		}
		
		if (errTimes >= 20) {  // ���մ����������20�ν�����USART3����
			errTimes = 0;
			car_DataPacket_Rx_Flag = 0;
			USART3_RX_STA = 0;
			USART3_RX_BUF[0] = 0;
		}
		
		// ���ջ�����������¿�ʼ����
		if ((USART3_RX_STA&USART3_RX_STA_REC_NUM)>(USART3_REC_LEN-1)) {
			printf("USART3 Rx Buff overflow!\r\n");
			// ��ս�������λ��ͳ��λ�����λ��λ
			USART3_RX_STA &= ~USART3_RX_STA_REC_NUM;
			USART3_RX_STA |=\
			((((USART3_RX_STA&USART3_RX_STA_OVERFLOW) >> 12) + 1)\
			<< 12) & USART3_RX_STA_OVERFLOW;
			// ��ս��ջ�����
			memset(USART3_RX_BUF, 0, USART3_REC_LEN);
		}
	}
}

#endif /* EN_USART3_RX */
