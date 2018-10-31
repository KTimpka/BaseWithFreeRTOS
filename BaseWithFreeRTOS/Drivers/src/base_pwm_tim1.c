/*
 * base_pwm_tim1.c
 *
 *  Created on: 14. okt 2018
 *      Author: Sad
 */
#include "base_pwm_tim1.h"

#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

static void base_pwm_tim1_thread(void *pvParameters);

void base_pwm_tim1_init()
{
	/*
	 * Configure GPIO
	 * B12 - 15 - Lower
	 * A8 - 11	- Higher
	 * Higer is PushPull, Lower PushPull or OpenDrain
	 * 2MHz good enough
	 * */
	//Enable Port A, B, Alternative clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//Pin config struct
	GPIO_InitTypeDef GPIO_Conf;

	//Configure PortA
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_Conf);

	//Configure PortB
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	//GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	//GPIO_Conf.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Conf);

	/*
	 * Confiugure TIM1 for PWM
	 * */
	//Enable TIM1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//TIM Config struct
	TIM_TimeBaseInitTypeDef TIM_Conf;

	TIM_Conf.TIM_Period 			=	0x3FFF;				//11b-35Khz, 12b-17Khz, 13b-8Khz, 14b-4Khz, 15b-2Khz, 16b-1Khz
	TIM_Conf.TIM_Prescaler			=	0x0000;				//No need for scaling
	TIM_Conf.TIM_ClockDivision		=	TIM_CKD_DIV4;		//For dead time 72/4 = 18MHz
	TIM_Conf.TIM_CounterMode		=	TIM_CounterMode_Up;	//Count up
	TIM_Conf.TIM_RepetitionCounter	=	0x00;				//Not used
	TIM_TimeBaseInit(TIM1, &TIM_Conf);

	//Start timer
	TIM_Cmd(TIM1, ENABLE);

	//Enable auto reload to preload registers, Auto reload register, Output compare preload
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	//Output compare Config struct
	TIM_OCInitTypeDef OC_Conf;

	OC_Conf.TIM_OCMode 			= TIM_OCMode_PWM1;			//Mode
	OC_Conf.TIM_OutputState		= TIM_OutputState_Enable;	//Use Positive channel
	OC_Conf.TIM_OutputNState	= TIM_OutputNState_Enable;	//Use Negative channel
	OC_Conf.TIM_Pulse			= 0x0000;		 			// Default will be 0
	OC_Conf.TIM_OCPolarity		= TIM_OCPolarity_High;		//IR2103S	High is positive on
	OC_Conf.TIM_OCNPolarity		= TIM_OCNPolarity_Low;		//IR2103S	Low is negative on
	OC_Conf.TIM_OCIdleState		= TIM_OCIdleState_Reset;	//Idle is low
	OC_Conf.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;	//Idle is low
	TIM_OC1Init(TIM1, &OC_Conf);
	TIM_OC2Init(TIM1, &OC_Conf);
	TIM_OC3Init(TIM1, &OC_Conf);
	TIM_OC4Init(TIM1, &OC_Conf);

	//Dead time Config struct
	TIM_BDTRInitTypeDef DT_Conf;

	TIM_BDTRStructInit(&DT_Conf);	//Default is fine
	DT_Conf.TIM_DeadTime		= 0x04;	//Dead time clock is 72/4=18Mhz, 1 step is 55,56ns, we need 200ns
	TIM_BDTRConfig(TIM1, &DT_Conf);

	//Start Capture/Compare modules
	TIM_CCxCmd(TIM1, TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3 | TIM_Channel_4, ENABLE);
	TIM_CCxNCmd(TIM1, TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3, ENABLE);

	//Enable timer outputs
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	xTaskCreate(base_pwm_tim1_thread, "Base_PWM", 200, 0, 0, NULL);
}

/*
 * FreeRTOS thread for PWM
 * */
static void base_pwm_tim1_thread(void *pvParameters)
{
	/*
	 * Thread delay value in MS
	 * */
	static TickType_t THREAD_DELAY;
	uint16_t value = 1, value_add = 0xA3;
	FunctionalState direction = ENABLE, uplow = ENABLE;

	uint16_t table[100];

	table[0] = 0;

	uint16_t i;
	for (i = 1; i < 100; ++i) {
		table[i] = table[i - 1] + value_add;
	}

	TIM_SetCompare1(TIM1, 0x0000);
	TIM_SetCompare2(TIM1, 0x0000);


	THREAD_DELAY = (TickType_t) (1 / portTICK_PERIOD_MS);

	//Thread loop
	while (1) {

		if (direction && uplow) {
			TIM_SetCompare1(TIM1, table[value]);
			value++;
		} else if (!direction && uplow) {
			TIM_SetCompare1(TIM1, table[value]);
			value--;
		} else if (direction && !uplow) {
			TIM_SetCompare2(TIM1, table[value]);
			value++;
		} else {
			TIM_SetCompare2(TIM1, table[value]);
			value--;
		}

		if (value <= 1) {
			TIM_SetCompare1(TIM1, table[0]);
			TIM_SetCompare2(TIM1, table[0]);
			direction = ENABLE;
			uplow ^= ENABLE;
		} else if (value >= 100) {
			direction = DISABLE;
			value--;
		}


		//Wait
		vTaskDelay(THREAD_DELAY);
	}

	//Never exit thread
	while (1);
}
