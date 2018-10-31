/*
 * base_thread.c
 *
 *  Created on: 8. okt 2018
 *      Author: Sad
 */

#include "base_thread.h"

#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/*
 * Thread delay value in MS
 * */
static TickType_t THREAD_DELAY;


static void base_thread_thread(void *pvParameters);



/*
 * Initialize blinky
 * */
void base_thread_init()
{

	THREAD_DELAY = (TickType_t) (100 / portTICK_PERIOD_MS);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);



	//Pin config struct
	GPIO_InitTypeDef GPIO_Conf;

	//, LED
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_Conf);
	GPIO_Write(GPIOB, 0xFFFF);

	xTaskCreate( base_thread_thread, "Base_Thread", configMINIMAL_STACK_SIZE, 0, 0, NULL );
}

/*
 * FreeRTOS thread for blinky
 * */
static void base_thread_thread(void *pvParameters)
{
	//Thread loop
	while (1) {
		uint16_t temp;
		uint16_t temp_2;

		//Read port state
		temp = GPIO_ReadOutputData(GPIOB);
		temp_2 = temp & (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		//Flip  state
		temp_2 ^= (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		temp &= ~(GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		temp |= temp_2;

		//Set new pin state
		GPIO_Write(GPIOB, temp);

		//Wait
		vTaskDelay(THREAD_DELAY);
	}

	//Never exit thread
	while (1);
}
