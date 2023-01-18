#include <string.h>
#include "usart.h"	  
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

////////////////////////////////////////////////////////////////// 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
//#pragma import(__use_no_semihosting)             
__asm (".global __use_no_semihosting\n\t");
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&USART_FLAG_TC)==0){}//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //如果使能了接收
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART1_RX_STA=0;       //接收状态标记	  
  
void USART1_Init(uint32_t bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure);		//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    	//使能串口1
	
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
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // 等待发送完成
		USART_SendData(USART1, *str);
		str++;
	}
	// 发送字符串末尾的'\0'
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // 等待发送完成
	USART_SendData(USART1, *str);
}

void USART1_Postback(void)
{
	if(USART1_RX_STA&0X8000) {  // 接收了数据
		USART1_Send_String(USART1_RX_BUF);
		memset(USART1_RX_BUF, 0, USART1_REC_LEN);
		USART1_RX_STA = 0;
	}
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}
     } 
} 
#endif /* EN_USART1_RX */

#if EN_USART3_RX   //如果使能了接收

#include <math.h>

/* 声明包数据 */
typedef struct Car_DataPacket_Rx Car_DataPacket_Rx_t;
typedef struct Car_DataPacket_Tx Car_DataPacket_Tx_t;

uint8_t  USART3_RX_BUF[USART3_REC_LEN];
//接收状态
//bit15，	接收但未读取(1)
//bit14，	接收溢出(1)
//bit13~0，	接收到的有效字节数目
uint16_t USART3_RX_STA = 0;

static uint32_t USART_Pow(uint32_t base, uint32_t index)
{
	uint32_t res = 1;
	
	while (index--) res *= base;
	
	return res;
}

void USART3_Init(uint32_t bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
  
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11 

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); 		//初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    	//使能串口3
	
	memset(USART3_RX_BUF, 0, USART3_REC_LEN);
	
	printf("\r\n");
	printf("***************************************************\r\n");
	printf("       STM32F103C8T6 USART3 has initialized!\r\n");
	printf("***************************************************\r\n");
	printf("\r\n");
}

void USART3_Send_Char(uint8_t chr)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // 等待发送完成
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
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // 等待发送完成
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
	// 发送字符串末尾的'\0'
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  // 等待发送完成
	USART_SendData(USART3, *str);
}

/**
 * @fn uint8_t USART3_Get_Chr(uint16_t addr)
 * @brief 获得接收缓存中的一个字符
 *
 * @param [addr] 获取的字符所在的位置
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
 * @brief 获得接收缓存中数据（字符串形式）
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
	if(USART3_RX_STA&USART3_RX_STA_HAS_REC) {  // 接收了数据
		USART3_Send_String(USART3_RX_BUF);
		memset(USART3_RX_BUF, 0, USART3_REC_LEN);
		USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
	}
}

void USART3_IRQHandler(void)                	// 串口3中断服务程序
{
	static uint8_t errTimes = 0;  // 数据包错误次数
	// 接收小车相关数据包 Car_DataPacket_Rx_t 的标志位
	// bit2~0	->	接收到的原始数据和校验和的字节数
	// bit3		->	接收到了包头数据
	// bit4		->	接收完了原始数据
	static uint8_t car_DataPacket_Rx_Flag = 0X00;
	uint8_t Res = 0;
	 
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {  // 接收中断
		Res =USART_ReceiveData(USART3);	// 读取接收到的数据
		USART3_RX_BUF[USART3_RX_STA&USART3_RX_STA_REC_NUM] = Res;
		USART3_RX_STA++;
		USART3_RX_BUF[USART3_RX_STA&USART3_RX_STA_REC_NUM] = 0;
		USART3_RX_STA |= USART3_RX_STA_HAS_REC;
		
		// 接收数据包
		// 接收未完成
		if (!(USART3_RX_STA&USART3_RX_STA_REC_END)) {
			if (car_DataPacket_Rx_Flag&0X08) {  // 接收到了包头数据
				// 接收原始数据和校验和
				if ((car_DataPacket_Rx_Flag&0X10) || ((car_DataPacket_Rx_Flag&0X07) == 4)) {  // 接收完成
					car_DataPacket_Rx_Flag |= 0X10;
					// 接收包尾数据
					if (Res == 0X5A) {  // 接收到了包尾数据
						USART3_RX_STA |= USART3_RX_STA_REC_END;
					} else {  // 数据包接收失败
						errTimes++;
						printf("BLE rec failed!\r\n");
						USART3_RX_STA &= USART3_RX_STA_OVERFLOW;
					}
					car_DataPacket_Rx_Flag = 0X00;
				} else car_DataPacket_Rx_Flag++;
			} else {
				if (Res == 0XA5)  // 正在接收的是包头数据
					car_DataPacket_Rx_Flag |= 0X08;
			}
		}
		
		if (errTimes >= 20) {  // 接收错误次数超过20次将重置USART3接收
			errTimes = 0;
			car_DataPacket_Rx_Flag = 0;
			USART3_RX_STA = 0;
			USART3_RX_BUF[0] = 0;
		}
		
		// 接收缓存溢出，重新开始接收
		if ((USART3_RX_STA&USART3_RX_STA_REC_NUM)>(USART3_REC_LEN-1)) {
			printf("USART3 Rx Buff overflow!\r\n");
			// 清空接收数据位数统计位，溢出位置位
			USART3_RX_STA &= ~USART3_RX_STA_REC_NUM;
			USART3_RX_STA |=\
			((((USART3_RX_STA&USART3_RX_STA_OVERFLOW) >> 12) + 1)\
			<< 12) & USART3_RX_STA_OVERFLOW;
			// 清空接收缓冲区
			memset(USART3_RX_BUF, 0, USART3_REC_LEN);
		}
	}
}

#endif /* EN_USART3_RX */
