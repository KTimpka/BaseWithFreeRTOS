/*
 * base_init.c
 *
 *  Created on: 16. apr 2018
 *      Author: Sad
 */

#include "base_init.h"

#include "system_stm32f10x.h"	//SystemInit()
#include "misc.h"				//NVIC
#include "core_cm3.h"			//SysTick

/*
 * Initialize basic hardware
 * */
void base_init()
{
	/*
	 * Set system clock to 72Mhz
	 * */

	SystemInit();

	/*
	 * Set NVIC to Flash and use 4 bits for pre-emption priority
	 * */

	//NVIC is on flash
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	//Use 0 bits for subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/*
	 * FreeRTOS uses SysTick to keep time
	 * Default is 1Khz
	 * */

	//SysTick reload value
	SysTick_Config(SYSTICK_RELOAD);
	//SysTick source is HCLK / 8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
