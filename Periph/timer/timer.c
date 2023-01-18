#include "timer.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void Timer1_Pwm_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1|
							RCC_APB2Periph_AFIO, ENABLE);  // PA11��TIM1_CH4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; // �������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; // ����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  // ��ʼ�� TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // ������Ը�
	TIM_OCInitStructure.TIM_Pulse = arr;  // CCR��Ĭ������Ϊarr����ռ�ձ�Ϊ0
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  // ��ʼ������ TIMx OC4
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  // ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE); // ��TIMx��ARR�Ĵ���װ�뻺����
	TIM_Cmd(TIM1, ENABLE);  // ʹ�� TIMx
}

void Timer2_Pwm_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // TIM2_CH4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
							RCC_APB2Periph_AFIO, ENABLE);  // PA3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; // �������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; // ����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  // ��ʼ�� TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // ������Ը�
	TIM_OCInitStructure.TIM_Pulse = arr;  // CCR��Ĭ������Ϊarr����ռ�ձ�Ϊ0
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  // ��ʼ������ TIMx OC4
	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  // ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE); // ��TIMx��ARR�Ĵ���װ�뻺����
	TIM_Cmd(TIM2, ENABLE);  // ʹ�� TIMx
}

void Timer3_Pwm_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM2_CH4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_AFIO, ENABLE);  // PB1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; // �������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; // ����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  // ��ʼ�� TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // ������Ը�
	TIM_OCInitStructure.TIM_Pulse = arr;  // CCR��Ĭ������Ϊarr����ռ�ձ�Ϊ0
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  // ��ʼ������ TIMx OC4
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  // ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE); // ��TIMx��ARR�Ĵ���װ�뻺����
	TIM_Cmd(TIM3, ENABLE);  // ʹ�� TIMx
}

void Timer4_Pwm_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  // TIM2_CH4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_AFIO, ENABLE);  // PB9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; // �������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; // ����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  // ��ʼ�� TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // ������Ը�
	TIM_OCInitStructure.TIM_Pulse = arr;  // CCR��Ĭ������Ϊarr����ռ�ձ�Ϊ0
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  // ��ʼ������ TIMx OC4
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  // ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE); // ��TIMx��ARR�Ĵ���װ�뻺����
	TIM_Cmd(TIM4, ENABLE);  // ʹ�� TIMx
}

/*=================================================
	TIMx->ARR
=================================================*/

void Set_TIMxCH4_Duty(TIM_TypeDef* TIMx, uint8_t duty)
{
	uint16_t compare = 0;
	
	assert_param(IS_TIM_LIST6_PERIPH(TIMx));
	if(duty > 100) return;
	
	duty = 100 - duty;
	compare = (uint16_t)(1.0 * duty * (TIMx->ARR + 1) / 100);
	if(compare != 0) compare--;
	TIM_SetCompare4(TIMx, compare);
}

uint8_t Get_TIMxCH4_Duty(TIM_TypeDef* TIMx)
{
	uint8_t duty = 0;
	uint16_t capture = 0;
	
	assert_param(IS_TIM_LIST6_PERIPH(TIMx));
	
	capture = TIM_GetCapture4(TIMx);
	duty = (uint8_t)(1.0 * (capture + 1) / (TIMx->ARR + 1) * 100);
	duty = 100 - duty;
	
	return duty;
}

/*=================================================
	TIM1
=================================================*/

void Set_TIM1CH4_Duty(uint8_t duty)
{
	Set_TIMxCH4_Duty(TIM1, duty);
}

uint8_t Get_TIM1CH4_Duty(void)
{
	return Get_TIMxCH4_Duty(TIM1);
}

/*=================================================
	TIM2
=================================================*/

void Set_TIM2CH4_Duty(uint8_t duty)
{
	Set_TIMxCH4_Duty(TIM2, duty);
}

uint8_t Get_TIM2CH4_Duty(void)
{
	return Get_TIMxCH4_Duty(TIM2);
}

/*=================================================
	TIM3
=================================================*/

void Set_TIM3CH4_Duty(uint8_t duty)
{
	Set_TIMxCH4_Duty(TIM3, duty);
}

uint8_t Get_TIM3CH4_Duty(void)
{
	return Get_TIMxCH4_Duty(TIM3);
}

/*=================================================
	TIM4
=================================================*/

void Set_TIM4CH4_Duty(uint8_t duty)
{
	Set_TIMxCH4_Duty(TIM4, duty);
}

uint8_t Get_TIM4CH4_Duty(void)
{
	return Get_TIMxCH4_Duty(TIM4);
}
