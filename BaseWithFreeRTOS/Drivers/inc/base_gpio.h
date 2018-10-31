/*
 * base_io.h
 *
 *  Created on: 12. okt 2018
 *      Author: Toftan2-elekter
 */

#ifndef INC_BASE_GPIO_H_
#define INC_BASE_GPIO_H_
/*
 * Includes
 * */

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

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

void base_gpio_init(GPIO_TypeDef * 		GPIOx, //Port A,B,C,D,E...
					uint16_t			GPIO_Pin, //Pin number 0 - 15
					GPIOSpeed_TypeDef 	GPIO_Speed, //Speed 2,10,50MHz
					GPIOMode_TypeDef 	GPIO_Mode, //Mode Ai, Di, Do, ...
					);

/*
 * NVIC functions
 * */

/*
 * Threads
 * */


#endif /* INC_BASE_GPIO_H_ */
