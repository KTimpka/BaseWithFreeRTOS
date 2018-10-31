/*
 * base.c
 *
 *  Created on: 13. okt 2018
 *      Author: Sad
 */

#include "base.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stdint.h"

/*
 * GPIO init function crammed into one function
 * */
void base_GPIO_init(GPIO_TypeDef* 		GPIOx,
					uint16_t			GPIO_Pin,
					GPIOSpeed_TypeDef 	GPIO_Speed,
					GPIOMode_TypeDef 	GPIO_Mode
					)
{
	/*
	 * Start clock
	 * */
	if (GPIOx == GPIOA)
	{	//Start PortA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}else if (GPIOx == GPIOB)
	{	//Start PortB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}else if (GPIOx == GPIOC)
	{	//Start PortC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}else
	{	//Smth is wrong STM32F103C8T6
		return;
	}

	//Pin config struct
	GPIO_InitTypeDef GPIO_Conf;

	//Fill struct
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode;
	GPIO_Init(GPIOx, &GPIO_Conf);

}
