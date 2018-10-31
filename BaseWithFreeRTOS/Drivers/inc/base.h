/*
 * base.h
 *
 *  Created on: 13. okt 2018
 *      Author: Sad
 */

#ifndef INC_BASE_H_
#define INC_BASE_H_
/*
 * Includes
 * */
#include "stm32f10x_gpio.h"

#include "stdint.h"

/*
 * Defines, macros
 * */

/*
 * GLOBAL
 * */

/*
 * Functions
 * */
void base_GPIO_init(GPIO_TypeDef* 		GPIOx,
					uint16_t			GPIO_Pin,
					GPIOSpeed_TypeDef 	GPIO_Speed,
					GPIOMode_TypeDef 	GPIO_Mode
					);
/*
 * NVIC functions
 * */

/*
 * Threads
 * */


#endif /* INC_BASE_H_ */
