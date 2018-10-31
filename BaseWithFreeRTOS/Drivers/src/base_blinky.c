/*
 * base_blinky.c
 *
 *  Created on: 16. apr 2018
 *      Author: Sad
 */

#include "base_blinky.h"

#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"


#include "stm32f10x_gpio.h"

/*
 * Thread delay value in MS
 * */
static TickType_t THREAD_DELAY;


static void base_blinky_thread(void *pvParameters);



/*
 * Initialize blinky
 * */
void base_blinky_init(uint32_t delay)
{
	/*
	 * Set delay for thread
	 * delay == 0 == 500MS
	 * */
	if (delay == 0)
		THREAD_DELAY = (TickType_t) (500 / portTICK_PERIOD_MS);
	else
		THREAD_DELAY = (TickType_t) (delay / portTICK_PERIOD_MS);

	//Enable Port C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//Pin config struct
	GPIO_InitTypeDef GPIO_Conf;

	//Configure PortA
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_13;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_Conf);

	xTaskCreate( base_blinky_thread, "Base_Blinky", configMINIMAL_STACK_SIZE, 0, 0, NULL );
}

/*
 * FreeRTOS thread for blinky
 * */
static void base_blinky_thread(void *pvParameters)
{
	//Thread loop
	while (1) {
		uint8_t temp;

		//Read pin state
		temp = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
		//Flip pin state
		temp ^= 1;
		//Set new pin state
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, temp);

		//Wait
		vTaskDelay(THREAD_DELAY);
	}

	//Never exit thread
	while (1);
}
