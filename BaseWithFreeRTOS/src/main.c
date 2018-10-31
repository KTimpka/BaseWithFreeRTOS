/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stdint.h"
#include "stddef.h"

#include "FreeRTOS.h"
#include "task.h"

#include "base_init.h"
#include "base_blinky.h"
#include "base_usart_1.h"
#include "base_WS2812B.h"
#include "base_thread.h"
#include "base_pwm_tim1.h"

#include "stm32f10x_tim.h"



int main(void)
{
	/*
	 * Run basic initialization
	 * */

	base_init();

	/*
	 * Drivers initialization with threads
	 * */
	base_blinky_init(100);
	//base_usart_1_init(NULL);
	//base_WS2812B_init();
	//base_thread_init();
	base_pwm_tim1_init();

	/*
	 * User threads initialization
	 * */

	/*
	 * Start FreeRTOS
	 * */
	vTaskStartScheduler();

	for(;;);
}
